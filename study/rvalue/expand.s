	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 0
	.globl	__Z6expandO5Point               ## -- Begin function _Z6expandO5Point
	.p2align	4, 0x90
__Z6expandO5Point:                      ## @_Z6expandO5Point
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %ecx
	addl	$1, %ecx
	movl	%ecx, (%rax)
	movq	-8(%rbp), %rax
	movl	4(%rax), %ecx
	addl	$1, %ecx
	movl	%ecx, 4(%rax)
	movq	-8(%rbp), %rax
	movl	8(%rax), %ecx
	addl	$1, %ecx
	movl	%ecx, 8(%rax)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	l___const.main.a(%rip), %rax
	movq	%rax, -12(%rbp)
	movl	l___const.main.a+8(%rip), %eax
	movl	%eax, -4(%rbp)
	leaq	-12(%rbp), %rdi
	callq	__Z6expandO5Point
	movq	(%rax), %rcx
	movq	%rcx, -24(%rbp)
	movl	8(%rax), %eax
	movl	%eax, -16(%rbp)
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__const
	.p2align	2, 0x0                          ## @__const.main.a
l___const.main.a:
	.long	1                               ## 0x1
	.long	2                               ## 0x2
	.long	3                               ## 0x3

.subsections_via_symbols
