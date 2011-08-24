#include <memlayout.h>
		
		.text

		.global __context_init
__context_init:
		movq 	$0, %rbp
		ret
		
		.global __context_switch

__context_switch:
		movq	%ss:8 (%esp), %rax   // from_esp_ptr
		movq    %ss:16(%esp), %rbx
		movq    %ss:24(%esp), %rcx   // to_esp
		movq    %ss:32(%esp), %rdx

		# no pusha in x86_64
#if CONTEXT_SWITCH_FEW_SAVE
		pushq %rbp
#else
		pushq %rdi
	    pushq %rsi
	    pushq %rdx
	    pushq %rcx
	    pushq %rax
	    pushq %r8
	    pushq %r9
	    pushq %r10
	    pushq %r11
	    pushq %rbx
	    pushq %rbp
	    pushq %r12
	    pushq %r13
	    pushq %r14
	    pushq %r15
#endif
		pushf
	
		movq	%rsp, 0(%rax)
		movabs  $__context_switch_ret, %rax
		movq    %rax, 0(%rbx)
		movq    %rcx, %rsp
		jmp     *%rdx

__context_switch_ret:
		popf
#if CONTEXT_SWITCH_FEW_SAVE
		popq %rbp
#else
	    popq %r15
	    popq %r14
	    popq %r13
	    popq %r12
	    popq %rbp
	    popq %rbx
	    popq %r11
	    popq %r10
	    popq %r9
	    popq %r8
	    popq %rax
	    popq %rcx
	    popq %rdx
	    popq %rsi
	    popq %rdi
#endif
		ret
		
