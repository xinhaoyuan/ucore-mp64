#include <mp/mp.h>

PLS int lapic_id;
PLS int lcpu_idx;
PLS int lcpu_count;

int
mp_init(void)
{
	lapic_id = lapic_id_get();
	lcpu_idx = lcpu_idx_get();
	lcpu_count = lcpu_count_get();

	return 0;
}
