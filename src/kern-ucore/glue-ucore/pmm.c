#include <pmm.h>
#include <slab.h>
#include <error.h>
#include <assert.h>
#include <x86.h>
#include <string.h>
#include <kio.h>
#include <swap.h>
#include <glue_mp.h>
#include <glue_intr.h>
#include <mp.h>

PLS static size_t used_pages;
PLS list_entry_t page_struct_free_list;

static struct Page *
page_struct_alloc(uintptr_t pa)
{
	if (list_empty(&page_struct_free_list))
	{
		struct Page *p = KADDR_DIRECT(kalloc_pages(1));
		
		int i;
		for (i = 0; i < PGSIZE / sizeof(struct Page); ++ i)
			list_add(&page_struct_free_list, &p[i].page_link);
	}

	list_entry_t *entry = list_next(&page_struct_free_list);
	list_del(entry);

	struct Page *page = le2page(entry, page_link);

	page->pa = pa;
	atomic_set(&page->ref, 0);
	page->flags = 0;
	list_init(entry);
	
	return page;
}

static void
page_struct_free(struct Page *page)
{
	list_add(&page_struct_free_list, &page->page_link);
}

struct Page *
alloc_pages(size_t npages)
{
	struct Page *result;
	uintptr_t base = kalloc_pages(npages);
	size_t i;
	int flags;

	local_intr_save_hw(flags);
	
	result = page_struct_alloc(base);
	kpage_private_set(base, result);

	for (i = 1; i < npages; ++ i)
	{
		struct Page *page = page_struct_alloc(base + i * PGSIZE);
		kpage_private_set(base + i * PGSIZE, page);
	}

	used_pages += npages;

	local_intr_restore_hw(flags);
	return result;
}

void
free_pages(struct Page *base, size_t npages)
{
	uintptr_t basepa = page2pa(base);
	size_t i;
	int flags;

	local_intr_save_hw(flags);
	
	for (i = 0; i < npages; ++ i)
	{
		page_struct_free(kpage_private_get(basepa + i * PGSIZE));
	}
	
	kfree_pages(basepa, npages);
	used_pages -= npages;

	local_intr_restore_hw(flags);
}

size_t
nr_used_pages(void)
{
	return used_pages;
}

void
pmm_init_ap(void)
{
	list_init(&page_struct_free_list);
	used_pages = 0;
}

pgd_t *
get_pgd(pgd_t *pgdir, uintptr_t la, bool create) {
    return &pgdir[PGX(la)];
}

pud_t *
get_pud(pgd_t *pgdir, uintptr_t la, bool create) {
    pgd_t *pgdp;
    if ((pgdp = get_pgd(pgdir, la, create)) == NULL) {
        return NULL;
    }
    if (!(*pgdp & PTE_P)) {
        struct Page *page;
        if (!create || (page = alloc_page()) == NULL) {
            return NULL;
        }
        set_page_ref(page, 1);
        uintptr_t pa = page2pa(page);
        memset(KADDR(pa), 0, PGSIZE);
        *pgdp = pa | PTE_U | PTE_W | PTE_P;
    }
    return &((pud_t *)KADDR(PGD_ADDR(*pgdp)))[PUX(la)];
}

pmd_t *
get_pmd(pgd_t *pgdir, uintptr_t la, bool create) {
    pud_t *pudp;
    if ((pudp = get_pud(pgdir, la, create)) == NULL) {
        return NULL;
    }
    if (!(*pudp & PTE_P)) {
        struct Page *page;
        if (!create || (page = alloc_page()) == NULL) {
            return NULL;
        }
        set_page_ref(page, 1);
        uintptr_t pa = page2pa(page);
        memset(KADDR(pa), 0, PGSIZE);
        *pudp = pa | PTE_U | PTE_W | PTE_P;
    }
    return &((pmd_t *)KADDR(PUD_ADDR(*pudp)))[PMX(la)];
}

pte_t *
get_pte(pgd_t *pgdir, uintptr_t la, bool create) {
    pmd_t *pmdp;
    if ((pmdp = get_pmd(pgdir, la, create)) == NULL) {
        return NULL;
    }
    if (!(*pmdp & PTE_P)) {
        struct Page *page;
        if (!create || (page = alloc_page()) == NULL) {
            return NULL;
        }
        set_page_ref(page, 1);
        uintptr_t pa = page2pa(page);
        memset(KADDR(pa), 0, PGSIZE);
        *pmdp = pa | PTE_U | PTE_W | PTE_P;
    }
    return &((pte_t *)KADDR(PMD_ADDR(*pmdp)))[PTX(la)];
}

struct Page *
get_page(pgd_t *pgdir, uintptr_t la, pte_t **ptep_store) {
    pte_t *ptep = get_pte(pgdir, la, 0);
    if (ptep_store != NULL) {
        *ptep_store = ptep;
    }
    if (ptep != NULL && *ptep & PTE_P) {
        return pa2page(*ptep);
    }
    return NULL;
}

static inline void
page_remove_pte(pgd_t *pgdir, uintptr_t la, pte_t *ptep) {
    if (*ptep & PTE_P) {
        struct Page *page = pte2page(*ptep);
        if (!PageSwap(page)) {
            if (page_ref_dec(page) == 0) {
                free_page(page);
            }
        }
        else {
            if (*ptep & PTE_D) {
                SetPageDirty(page);
            }
            page_ref_dec(page);
        }
        *ptep = 0;
        mp_tlb_invalidate(pgdir, la);
    }
    else if (*ptep != 0) {
        swap_remove_entry(*ptep);
        *ptep = 0;
    }
}

void
page_remove(pgd_t *pgdir, uintptr_t la) {
    pte_t *ptep = get_pte(pgdir, la, 0);
    if (ptep != NULL) {
        page_remove_pte(pgdir, la, ptep);
    }
}

int
page_insert(pgd_t *pgdir, struct Page *page, uintptr_t la, uint32_t perm) {
    pte_t *ptep = get_pte(pgdir, la, 1);
    if (ptep == NULL) {
        return -E_NO_MEM;
    }
    page_ref_inc(page);
    if (*ptep != 0) {
        if ((*ptep & PTE_P) && pte2page(*ptep) == page) {
            page_ref_dec(page);
            goto out;
        }
        page_remove_pte(pgdir, la, ptep);
    }

out:
    *ptep = page2pa(page) | PTE_P | perm;
    mp_tlb_invalidate(pgdir, la);
    return 0;
}

// invalidate a TLB entry, but only if the page tables being
// edited are the ones currently in use by the processor.
void
tlb_invalidate(pgd_t *pgdir, uintptr_t la) {
    if (rcr3() == PADDR(pgdir)) {
        invlpg((void *)la);
    }
}

struct Page *
pgdir_alloc_page(pgd_t *pgdir, uintptr_t la, uint32_t perm) {
    struct Page *page = alloc_page();
    if (page != NULL) {
        if (page_insert(pgdir, page, la, perm) != 0) {
            free_page(page);
            return NULL;
        }
    }
    return page;
}

static void
unmap_range_pte(pgd_t *pgdir, pte_t *pte, uintptr_t base, uintptr_t start, uintptr_t end) {
    assert(start >= 0 && start < end && end <= PTSIZE);
    assert(start % PGSIZE == 0 && end % PGSIZE == 0);
    do {
        pte_t *ptep = &pte[PTX(start)];
        if (*ptep != 0) {
            page_remove_pte(pgdir, base + start, ptep);
        }
        start += PGSIZE;
    } while (start != 0 && start < end);
}

static void
unmap_range_pmd(pgd_t *pgdir, pmd_t *pmd, uintptr_t base, uintptr_t start, uintptr_t end) {
    assert(start >= 0 && start < end && end <= PMSIZE);
    size_t off, size;
    uintptr_t la = ROUNDDOWN(start, PTSIZE);
    do {
        off = start - la, size = PTSIZE - off;
        if (size > end - start) {
            size = end - start;
        }
        pmd_t *pmdp = &pmd[PMX(la)];
        if (*pmdp & PTE_P) {
            unmap_range_pte(pgdir, KADDR(PMD_ADDR(*pmdp)), base + la, off, off + size);
        }
        start += size, la += PTSIZE;
    } while (start != 0 && start < end);
}

static void
unmap_range_pud(pgd_t *pgdir, pud_t *pud, uintptr_t base, uintptr_t start, uintptr_t end) {
    assert(start >= 0 && start < end && end <= PUSIZE);
    size_t off, size;
    uintptr_t la = ROUNDDOWN(start, PMSIZE);
    do {
        off = start - la, size = PMSIZE - off;
        if (size > end - start) {
            size = end - start;
        }
        pud_t *pudp = &pud[PUX(la)];
        if (*pudp & PTE_P) {
            unmap_range_pmd(pgdir, KADDR(PUD_ADDR(*pudp)), base + la, off, off + size);
        }
        start += size, la += PMSIZE;
    } while (start != 0 && start < end);
}

static void
unmap_range_pgd(pgd_t *pgd, uintptr_t start, uintptr_t end) {
    size_t off, size;
    uintptr_t la = ROUNDDOWN(start, PUSIZE);
    do {
        off = start - la, size = PUSIZE - off;
        if (size > end - start) {
            size = end - start;
        }
        pgd_t *pgdp = &pgd[PGX(la)];
        if (*pgdp & PTE_P) {
            unmap_range_pud(pgd, KADDR(PGD_ADDR(*pgdp)), la, off, off + size);
        }
        start += size, la += PUSIZE;
    } while (start != 0 && start < end);
}

void
unmap_range(pgd_t *pgdir, uintptr_t start, uintptr_t end) {
    assert(start % PGSIZE == 0 && end % PGSIZE == 0);
    assert(USER_ACCESS(start, end));
    unmap_range_pgd(pgdir, start, end);
}

static void
exit_range_pmd(pmd_t *pmd) {
    uintptr_t la = 0;
    do {
        pmd_t *pmdp = &pmd[PMX(la)];
        if (*pmdp & PTE_P) {
            free_page(pmd2page(*pmdp)), *pmdp = 0;
        }
        la += PTSIZE;
    } while (la != PMSIZE);
}

static void
exit_range_pud(pud_t *pud) {
    uintptr_t la = 0;
    do {
        pud_t *pudp = &pud[PUX(la)];
        if (*pudp & PTE_P) {
            exit_range_pmd(KADDR(PUD_ADDR(*pudp)));
            free_page(pud2page(*pudp)), *pudp = 0;
        }
        la += PMSIZE;
    } while (la != PUSIZE);
}

static void
exit_range_pgd(pgd_t *pgd, uintptr_t start, uintptr_t end) {
    start = ROUNDDOWN(start, PUSIZE);
    do {
        pgd_t *pgdp = &pgd[PGX(start)];
        if (*pgdp & PTE_P) {
            exit_range_pud(KADDR(PGD_ADDR(*pgdp)));
            free_page(pgd2page(*pgdp)), *pgdp = 0;
        }
        start += PUSIZE;
    } while (start != 0 && start < end);
}

void
exit_range(pgd_t *pgdir, uintptr_t start, uintptr_t end) {
    assert(start % PGSIZE == 0 && end % PGSIZE == 0);
    assert(USER_ACCESS(start, end));
    exit_range_pgd(pgdir, start, end);
}

int
copy_range(pgd_t *to, pgd_t *from, uintptr_t start, uintptr_t end, bool share) {
    assert(start % PGSIZE == 0 && end % PGSIZE == 0);
    assert(USER_ACCESS(start, end));

    do { 
        pte_t *ptep = get_pte(from, start, 0), *nptep;
        if (ptep == NULL) {
            if (get_pud(from, start, 0) == NULL) {
                start = ROUNDDOWN(start + PUSIZE, PUSIZE);
            }
            else if (get_pmd(from, start, 0) == NULL) {
                start = ROUNDDOWN(start + PMSIZE, PMSIZE);
            }
            else {
                start = ROUNDDOWN(start + PTSIZE, PTSIZE);
            }
            continue ;
        }
        if (*ptep != 0) {
            if ((nptep = get_pte(to, start, 1)) == NULL) {
                return -E_NO_MEM;
            }
            int ret;
            assert(*ptep != 0 && *nptep == 0);
            if (*ptep & PTE_P) {
                uint32_t perm = (*ptep & PTE_USER);
                struct Page *page = pte2page(*ptep);
                if (!share && (*ptep & PTE_W)) {
                    perm &= ~PTE_W;
                    page_insert(from, page, start, perm | (*ptep & PTE_SWAP));
                }
                ret = page_insert(to, page, start, perm);
                assert(ret == 0);
            }
            else {
                swap_entry_t entry = *ptep;
                swap_duplicate(entry);
                *nptep = entry;
            }
        }
        start += PGSIZE;
    } while (start != 0 && start < end);
    return 0;
}

//perm2str - use string 'u,r,w,-' to present the permission
static const char *
perm2str(int perm) {
    static char str[4];
    str[0] = (perm & PTE_U) ? 'u' : '-';
    str[1] = 'r';
    str[2] = (perm & PTE_W) ? 'w' : '-';
    str[3] = '\0';
    return str;
}

//get_pgtable_items - In [left, right] range of PDT or PT, find a continuous linear addr space
//                  - (left_store*X_SIZE~right_store*X_SIZE) for PDT or PT
//                  - X_SIZE=PTSIZE=4M, if PDT; X_SIZE=PGSIZE=4K, if PT
// paramemters:
//  left:        no use ???
//  right:       the high side of table's range
//  start:       the low side of table's range
//  table:       the beginning addr of table
//  left_store:  the pointer of the high side of table's next range
//  right_store: the pointer of the low side of table's next range
// return value: 0 - not a invalid item range, perm - a valid item range with perm permission 
static int
get_pgtable_items(size_t left, size_t right, size_t start, uintptr_t *table, size_t *left_store, size_t *right_store) {
    if (start >= right) {
        return 0;
    }
    while (start < right && !(table[start] & PTE_P)) {
        start ++;
    }
    if (start < right) {
        if (left_store != NULL) {
            *left_store = start;
        }
        int perm = (table[start ++] & PTE_USER);
        while (start < right && (table[start] & PTE_USER) == perm) {
            start ++;
        }
        if (right_store != NULL) {
            *right_store = start;
        }
        return perm;
    }
    return 0;
}

static void
print_pgdir_sub(int deep, size_t left, size_t right, char *s1[], size_t s2[], uintptr_t *s3[]) {
    if (deep > 0) {
        uint32_t perm;
        size_t l, r = left;
        while ((perm = get_pgtable_items(left, right, r, s3[0], &l, &r)) != 0) {
            kprintf(s1[0], r - l);
            size_t lb = l * s2[0], rb = r * s2[0];
            if ((lb >> 32) & 0x8000) {
                lb |= (0xFFFFLLU << 48);
                rb |= (0xFFFFLLU << 48);
            }
            kprintf(" %016llx-%016llx %016llx %s\n", lb, rb, rb - lb, perm2str(perm));
            print_pgdir_sub(deep - 1, l * NPGENTRY, r * NPGENTRY, s1 + 1, s2 + 1, s3 + 1);
        }
    }
}

pte_t * const vpt = (pte_t *)VPT;
pmd_t * const vmd = (pmd_t *)PGADDR(PGX(VPT), PGX(VPT), 0, 0, 0);
pud_t * const vud = (pud_t *)PGADDR(PGX(VPT), PGX(VPT), PGX(VPT), 0, 0);
pgd_t * const vgd = (pgd_t *)PGADDR(PGX(VPT), PGX(VPT), PGX(VPT), PGX(VPT), 0);

//print_pgdir - print the PDT&PT
void
print_pgdir(void) {
    char *s1[] = {
        "PGD          (%09x)",
        " |-PUD       (%09x)",
        " |--|-PMD    (%09x)",
        " |--|--|-PTE (%09x)",
    };
    size_t s2[] = {PUSIZE, PMSIZE, PTSIZE, PGSIZE};
    uintptr_t *s3[] = {vgd, vud, vmd, vpt};
    kprintf("-------------------- BEGIN --------------------\n");
    print_pgdir_sub(sizeof(s1) / sizeof(s1[0]), 0, NPGENTRY, s1, s2, s3);
    kprintf("--------------------- END ---------------------\n");
}
