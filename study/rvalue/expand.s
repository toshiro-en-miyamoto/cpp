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
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %ecx
	incl	%ecx
	movl	%ecx, (%rax)
	movq	-24(%rbp), %rax
	movl	4(%rax), %ecx
	incl	%ecx
	movl	%ecx, 4(%rax)
	movq	-24(%rbp), %rax
	movl	8(%rax), %ecx
	incl	%ecx
	movl	%ecx, 8(%rax)
	movq	-24(%rbp), %rax
	movl	8(%rax), %ecx
	movl	%ecx, -8(%rbp)
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movl	-32(%rbp), %edx
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
	subq	$48, %rsp
	movabsq	$8589934593, %rax               ## imm = 0x200000001
	movq	%rax, -16(%rbp)
	movl	$3, -8(%rbp)
	leaq	-16(%rbp), %rdi
	callq	__Z6expandO5Point
	movl	%edx, -40(%rbp)
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -28(%rbp)
	movl	-40(%rbp), %eax
	movl	%eax, -20(%rbp)
	xorl	%eax, %eax
	addq	$48, %rsp
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
