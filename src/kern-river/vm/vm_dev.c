#define __V3VEE__

#include <palacios/vmm.h>
#include <palacios/vmm_dev_mgr.h>
#include <palacios/vmm_hypercall.h>
#include <palacios/vm_guest_mem.h>

#include <vm/vm_dev.h>

static struct v3_device_ops dev_ops =
{
	.free = NULL,
	.reset = NULL,
    .start = NULL,
    .stop = NULL,
};

static int
vm_dev_hcall(struct guest_info * info, uint32_t hcall_id, void *priv_data) {
    return 0;
}

void *
vm_dev_init(struct guest_info *vm, unsigned int buf_psize)
{
	char name[] = "vm_dev";
	
    struct vm_device * dev = v3_allocate_device(name, &dev_ops, NULL);
	if (v3_attach_device(vm, dev) == -1)
	{
		V3_Print("vm_dev: Could not attach device %s\n", name);
		return NULL;
    }

	addr_t buf = (addr_t)V3_AllocPages(buf_psize);

	v3_register_hypercall(vm, VM_DEV_CALLBACK_HCALL, vm_dev_hcall, dev);
	v3_add_shadow_mem(vm,
					  VM_DEV_SHADOW_MEM_START,
					  VM_DEV_SHADOW_MEM_START + buf_psize * PAGE_SIZE - 1,
					  buf);

	void *result;
	host_pa_to_host_va(buf, (addr_t *)&result);

	return result;
}
