#ifndef __LAPIC_H__
#define __LAPIC_H__

#include <types.h>
#include <mmu.h>
#include <pmm.h>

#define LAPIC_COUNT 256

typedef struct lcpu_static_s
{
	pgd_t *init_pgdir;
} lcpu_static_s;

typedef struct lcpu_dynamic_s
{
} lcpu_dynamic_s;

extern unsigned int   lcpu_id_set[LAPIC_COUNT];
/* Indexed by APIC ID */
extern lcpu_static_s  lcpu_static[LAPIC_COUNT];
extern lcpu_dynamic_s lcpu_dynamic[LAPIC_COUNT];

int  lapic_init(void);
int  lapic_init_ap(void);
int  lapic_id();
void lapic_eoi(void);
void lapic_start_ap(int apicid, uint32_t addr);
void lapic_set_timer(uint32_t freq);


#endif
