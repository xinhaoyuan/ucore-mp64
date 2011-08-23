extern void (*cprintf)(char *, int);
extern int  (*lapic_id)(void);

void
__kern_entry(void)
{
	cprintf("HELLO FROM %d!\n", lapic_id());
	while (1) ;
}
