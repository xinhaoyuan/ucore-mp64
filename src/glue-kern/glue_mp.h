#ifndef __GLUE_MP_H__
#define __GLUE_MP_H__

#ifdef LIBSPREFIX
#include <libs/types.h>
#else
#include <types.h>
#endif

typedef struct lcpu_info_s
{
	int      lapic_id;
	int      idx;
	
	uint64_t freq;
} lcpu_info_s;

#define lapic_id_get     (*lapic_id_get_ptr)
#define lcpu_idx_get     (*lcpu_idx_get_ptr)
#define lcpu_count_get   (*lcpu_count_get_ptr)
#define lapic_ipi_issue  (*lapic_ipi_issue_ptr)

extern unsigned int lapic_id_get(void);
extern unsigned int lcpu_idx_get(void);
extern unsigned int lcpu_count_get(void);
extern int          lapic_ipi_issue(int lapic_id);

#define LAPIC_COUNT 256

#define PLS __attribute__((section(".pls")))

#endif
