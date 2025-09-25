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
	callq	__ZN11ring_bufferC1Ev
	movl	$0, -60(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$7, -60(%rbp)
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-60(%rbp), %esi
	leaq	-56(%rbp), %rdi
	callq	__ZN11ring_buffer4pushEi
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-60(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -60(%rbp)
	jmp	LBB0_1
LBB0_4:
	jmp	LBB0_5
LBB0_5:                                 ## =>This Inner Loop Header: Depth=1
	leaq	-56(%rbp), %rdi
	callq	__ZNK11ring_buffer8is_emptyEv
	xorb	$-1, %al
	testb	$1, %al
	jne	LBB0_6
	jmp	LBB0_7
LBB0_6:                                 ##   in Loop: Header=BB0_5 Depth=1
	leaq	-56(%rbp), %rdi
	callq	__ZN11ring_buffer3popEv
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
	.globl	__ZN11ring_bufferC1Ev           ## -- Begin function _ZN11ring_bufferC1Ev
	.weak_def_can_be_hidden	__ZN11ring_bufferC1Ev
	.p2align	4, 0x90
__ZN11ring_bufferC1Ev:                  ## @_ZN11ring_bufferC1Ev
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZN11ring_bufferC2Ev
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZNK11ring_buffer8is_emptyEv   ## -- Begin function _ZNK11ring_buffer8is_emptyEv
	.weak_definition	__ZNK11ring_buffer8is_emptyEv
	.p2align	4, 0x90
__ZNK11ring_buffer8is_emptyEv:          ## @_ZNK11ring_buffer8is_emptyEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	cmpq	$0, 40(%rax)
	sete	%al
	andb	$1, %al
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZN11ring_bufferC2Ev           ## -- Begin function _ZN11ring_bufferC2Ev
	.weak_def_can_be_hidden	__ZN11ring_bufferC2Ev
	.p2align	4, 0x90
__ZN11ring_bufferC2Ev:                  ## @_ZN11ring_bufferC2Ev
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
	movq	$0, 32(%rax)
	movq	$0, 40(%rax)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d\n"

.subsections_via_symbols
