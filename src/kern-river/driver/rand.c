#include <driver/rand.h>
#include <driver/hpet.h>
#include <intr.h>
#include <mp.h>

PLS static uint32_t seed;

int
rand_init(void)
{
	seed = *hpet_tick;
	return 0;
}

uint16_t
rand16(void)
{
	uint16_t r;
	int flag;
	local_intr_save_hw(flag);
	r = seed = seed * 1103515245 + 12345;
	local_intr_restore_hw(flag);
	return r;
}
