#ifndef __GLUE_PMM_H__
#define __GLUE_PMM_H__

#include <mmu.h>
#include <memlayout.h>
#include <types.h>
#include <assert.h>
#include <kio.h>

#define kalloc_pages (*kalloc_pages_ptr)
#define kfree_pages (*kfree_pages_ptr)
#define kpage_private_set (*kpage_private_set_ptr)
#define kpage_private_get (*kpage_private_get_ptr)
#define load_rsp0 (*load_rsp0_ptr)
#define init_pgdir_get (*init_pgdir_get_ptr)

extern uintptr_t kalloc_pages(size_t npages);
extern void      kfree_pages(uintptr_t basepa, size_t npages);
extern void      kpage_private_set(uintptr_t pa, void *private);
extern void     *kpage_private_get(uintptr_t pa);
extern void      load_rsp0(uintptr_t rsp0);
extern pgd_t    *init_pgdir_get(void);

/* Simply translate between VA and PA without checking */
#define KADDR(addr) ((void*)((uintptr_t)(addr) + KERNBASE))
#define PADDR(addr) ((uintptr_t)(addr) - KERNBASE)

#define NEXT_PAGE(pg) (pa2page(page2pa(pg) + PGSIZE))

static inline ppn_t
page2ppn(struct Page *page) {
    return PPN(page->pa);
}

static inline uintptr_t
page2pa(struct Page *page) {
    return page->pa;
}

static inline struct Page *
pa2page(uintptr_t pa) {
	return (struct Page *)(kpage_private_get(pa));
}

static inline void *
page2kva(struct Page *page) {
    return KADDR(page2pa(page));
}

static inline struct Page *
kva2page(void *kva) {
    return pa2page(PADDR(kva));
}

static inline struct Page *
pte2page(pte_t pte) {
    if (!(pte & PTE_P)) {
        panic("pte2page called with invalid pte");
    }
    return pa2page(PTE_ADDR(pte));
}

static inline struct Page *
pmd2page(pmd_t pmd) {
    return pa2page(PMD_ADDR(pmd));
}

static inline struct Page *
pud2page(pud_t pud) {
    return pa2page(PUD_ADDR(pud));
}

static inline struct Page *
pgd2page(pgd_t pgd) {
    return pa2page(PGD_ADDR(pgd));
}

static inline int
page_ref(struct Page *page) {
    return atomic_read(&(page->ref));
}

static inline void
set_page_ref(struct Page *page, int val) {
    atomic_set(&(page->ref), val);
}

static inline int
page_ref_inc(struct Page *page) {
    return atomic_add_return(&(page->ref), 1);
}

static inline int
page_ref_dec(struct Page *page) {
    return atomic_sub_return(&(page->ref), 1);
}

pgd_t *get_pgd(pgd_t *pgdir, uintptr_t la, bool create);
pud_t *get_pud(pgd_t *pgdir, uintptr_t la, bool create);
pmd_t *get_pmd(pgd_t *pgdir, uintptr_t la, bool create);
pte_t *get_pte(pgd_t *pgdir, uintptr_t la, bool create);

#define alloc_page() alloc_pages(1)
#define free_page(page) free_pages(page, 1)

struct Page *        alloc_pages(size_t npages);
void         free_pages(struct Page *base, size_t npages);
size_t       nr_used_pages(void);

struct Page *get_page(pgd_t *pgdir, uintptr_t la, pte_t **ptep_store);
void page_remove(pgd_t *pgdir, uintptr_t la);
int page_insert(pgd_t *pgdir, struct Page *page, uintptr_t la, uint32_t perm);

void tlb_invalidate(pgd_t *pgdir, uintptr_t la);
struct Page *pgdir_alloc_page(pgd_t *pgdir, uintptr_t la, uint32_t perm);
void unmap_range(pgd_t *pgdir, uintptr_t start, uintptr_t end);
void exit_range(pgd_t *pgdir, uintptr_t start, uintptr_t end);
int copy_range(pgd_t *to, pgd_t *from, uintptr_t start, uintptr_t end, bool share);

void print_pgdir(void);

void pmm_init(void);

#endif
