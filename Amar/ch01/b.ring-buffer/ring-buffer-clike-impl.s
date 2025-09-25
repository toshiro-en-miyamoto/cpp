	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 0
	.globl	__Z20int_ring_buffer_initP15int_ring_buffer ## -- Begin function _Z20int_ring_buffer_initP15int_ring_buffer
	.p2align	4, 0x90
__Z20int_ring_buffer_initP15int_ring_buffer: ## @_Z20int_ring_buffer_initP15int_ring_buffer
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$0, 24(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 32(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 40(%rax)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__Z20int_ring_buffer_pushP15int_ring_bufferi ## -- Begin function _Z20int_ring_buffer_pushP15int_ring_bufferi
	.p2align	4, 0x90
__Z20int_ring_buffer_pushP15int_ring_bufferi: ## @_Z20int_ring_buffer_pushP15int_ring_bufferi
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	-12(%rbp), %edx
	movq	-8(%rbp), %rax
	movq	-8(%rbp), %rcx
	movq	24(%rcx), %rcx
	movl	%edx, (%rax,%rcx,4)
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-8(%rbp), %rax
	movq	%rdx, 24(%rax)
	movq	-8(%rbp), %rax
	cmpq	$5, 40(%rax)
	jae	LBB1_2
## %bb.1:
	movq	-8(%rbp), %rax
	movq	40(%rax), %rcx
	addq	$1, %rcx
	movq	%rcx, 40(%rax)
	jmp	LBB1_3
LBB1_2:
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-8(%rbp), %rax
	movq	%rdx, 32(%rax)
LBB1_3:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__Z19int_ring_buffer_popP15int_ring_buffer ## -- Begin function _Z19int_ring_buffer_popP15int_ring_buffer
	.p2align	4, 0x90
__Z19int_ring_buffer_popP15int_ring_buffer: ## @_Z19int_ring_buffer_popP15int_ring_buffer
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -16(%rbp)
	movq	-16(%rbp), %rax
	cmpq	$0, 40(%rax)
	jne	LBB2_2
## %bb.1:
	movl	$0, -4(%rbp)
	jmp	LBB2_3
LBB2_2:
	movq	-16(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	32(%rcx), %rcx
	movl	(%rax,%rcx,4), %eax
	movl	%eax, -20(%rbp)
	movq	-16(%rbp), %rax
	movq	32(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-16(%rbp), %rax
	movq	%rdx, 32(%rax)
	movq	-16(%rbp), %rax
	movq	40(%rax), %rcx
	addq	$-1, %rcx
	movq	%rcx, 40(%rax)
	movl	-20(%rbp), %eax
	movl	%eax, -4(%rbp)
LBB2_3:
	movl	-4(%rbp), %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
