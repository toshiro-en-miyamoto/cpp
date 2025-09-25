	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 0
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
	subq	$64, %rsp
	movl	$0, -4(%rbp)
	leaq	-56(%rbp), %rdi
	callq	__Z20int_ring_buffer_initP15int_ring_buffer
	movl	$0, -60(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$7, -60(%rbp)
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-60(%rbp), %esi
	leaq	-56(%rbp), %rdi
	callq	__Z20int_ring_buffer_pushP15int_ring_bufferi
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-60(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -60(%rbp)
	jmp	LBB0_1
LBB0_4:
	jmp	LBB0_5
LBB0_5:                                 ## =>This Inner Loop Header: Depth=1
	cmpq	$0, -16(%rbp)
	jbe	LBB0_7
## %bb.6:                               ##   in Loop: Header=BB0_5 Depth=1
	leaq	-56(%rbp), %rdi
	callq	__Z19int_ring_buffer_popP15int_ring_buffer
	movl	%eax, -64(%rbp)
	movl	-64(%rbp), %esi
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	jmp	LBB0_5
LBB0_7:
	movl	-4(%rbp), %eax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d\n"

.subsections_via_symbols
