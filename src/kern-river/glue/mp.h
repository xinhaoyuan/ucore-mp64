#ifndef __GLUE_MP_H__
#define __GLUE_MP_H__

#define lapic_id_get   (*lapic_id_get_ptr)
#define lcpu_idx_get   (*lcpu_idx_get_ptr)
#define lcpu_count_get (*lcpu_count_get_ptr)

extern unsigned int lapic_id_get(void);
extern unsigned int lcpu_idx_get(void);
extern unsigned int lcpu_count_get(void);

#define PLS __attribute__((section(".pls")))

#endif
