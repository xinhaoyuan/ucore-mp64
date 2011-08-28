#ifndef __GLUE_MP_H__
#define __GLUE_MP_H__

#define lapic_id   (*lapic_id_ptr)
#define lcpu_idx   (*lcpu_idx_ptr)
#define lcpu_count (*lcpu_count_ptr)

extern unsigned int lapic_id(void);
extern unsigned int lcpu_idx(void);
extern unsigned int lcpu_count(void);

#endif
