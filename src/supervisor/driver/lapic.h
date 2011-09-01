#ifndef __LAPIC_H__
#define __LAPIC_H__

#include <types.h>
#include <mmu.h>
#include <pmm.h>

#define LAPIC_COUNT 256

int  lapic_init(void);
int  lapic_init_ap(void);
int  lapic_id();
void lapic_send_eoi(void);
void lapic_start_ap(int apicid, uint32_t addr);
void lapic_set_timer(uint32_t freq);
int  lapic_ipi_issue(int lapic_id);

#endif
