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
	callq	__ZN11ring_bufferIiLm5EEC1Ev
	movl	$0, -60(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$7, -60(%rbp)
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-60(%rbp), %esi
	leaq	-56(%rbp), %rdi
	callq	__ZN11ring_bufferIiLm5EE4pushEi
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-60(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -60(%rbp)
	jmp	LBB0_1
LBB0_4:
	jmp	LBB0_5
LBB0_5:                                 ## =>This Inner Loop Header: Depth=1
	leaq	-56(%rbp), %rdi
	callq	__ZNK11ring_bufferIiLm5EE8is_emptyEv
	xorb	$-1, %al
	testb	$1, %al
	jne	LBB0_6
	jmp	LBB0_7
LBB0_6:                                 ##   in Loop: Header=BB0_5 Depth=1
	leaq	-56(%rbp), %rdi
	callq	__ZN11ring_bufferIiLm5EE3popEv
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
	.globl	__ZN11ring_bufferIiLm5EEC1Ev    ## -- Begin function _ZN11ring_bufferIiLm5EEC1Ev
	.weak_def_can_be_hidden	__ZN11ring_bufferIiLm5EEC1Ev
	.p2align	4, 0x90
__ZN11ring_bufferIiLm5EEC1Ev:           ## @_ZN11ring_bufferIiLm5EEC1Ev
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
	callq	__ZN11ring_bufferIiLm5EEC2Ev
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZN11ring_bufferIiLm5EE4pushEi ## -- Begin function _ZN11ring_bufferIiLm5EE4pushEi
	.weak_definition	__ZN11ring_bufferIiLm5EE4pushEi
	.p2align	4, 0x90
__ZN11ring_bufferIiLm5EE4pushEi:        ## @_ZN11ring_bufferIiLm5EE4pushEi
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rdi
	movq	%rdi, -24(%rbp)                 ## 8-byte Spill
	movl	-12(%rbp), %eax
	movl	%eax, -28(%rbp)                 ## 4-byte Spill
	movq	24(%rdi), %rsi
	callq	__ZNSt3__15arrayIiLm5EE2atB8ne200100Em
	movl	-28(%rbp), %edx                 ## 4-byte Reload
	movq	%rax, %rcx
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movl	%edx, (%rcx)
	movq	24(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	%rdx, 24(%rax)
	cmpq	$5, 40(%rax)
	jae	LBB2_2
## %bb.1:
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	40(%rax), %rcx
	addq	$1, %rcx
	movq	%rcx, 40(%rax)
	jmp	LBB2_3
LBB2_2:
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	32(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	%rdx, 32(%rax)
LBB2_3:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZNK11ring_bufferIiLm5EE8is_emptyEv ## -- Begin function _ZNK11ring_bufferIiLm5EE8is_emptyEv
	.weak_definition	__ZNK11ring_bufferIiLm5EE8is_emptyEv
	.p2align	4, 0x90
__ZNK11ring_bufferIiLm5EE8is_emptyEv:   ## @_ZNK11ring_bufferIiLm5EE8is_emptyEv
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
	.globl	__ZN11ring_bufferIiLm5EE3popEv  ## -- Begin function _ZN11ring_bufferIiLm5EE3popEv
	.weak_definition	__ZN11ring_bufferIiLm5EE3popEv
	.p2align	4, 0x90
__ZN11ring_bufferIiLm5EE3popEv:         ## @_ZN11ring_bufferIiLm5EE3popEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	cmpq	$0, 40(%rax)
	jne	LBB4_2
## %bb.1:
	movl	$0, -4(%rbp)
	jmp	LBB4_3
LBB4_2:
	movq	-32(%rbp), %rdi                 ## 8-byte Reload
	movq	32(%rdi), %rsi
	callq	__ZNSt3__15arrayIiLm5EE2atB8ne200100Em
	movq	%rax, %rcx
	movq	-32(%rbp), %rax                 ## 8-byte Reload
	movl	(%rcx), %ecx
	movl	%ecx, -20(%rbp)
	movq	32(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-32(%rbp), %rax                 ## 8-byte Reload
	movq	%rdx, 32(%rax)
	movq	40(%rax), %rcx
	addq	$-1, %rcx
	movq	%rcx, 40(%rax)
	movl	-20(%rbp), %eax
	movl	%eax, -4(%rbp)
LBB4_3:
	movl	-4(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZN11ring_bufferIiLm5EEC2Ev    ## -- Begin function _ZN11ring_bufferIiLm5EEC2Ev
	.weak_def_can_be_hidden	__ZN11ring_bufferIiLm5EEC2Ev
	.p2align	4, 0x90
__ZN11ring_bufferIiLm5EEC2Ev:           ## @_ZN11ring_bufferIiLm5EEC2Ev
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
	.private_extern	__ZNSt3__15arrayIiLm5EE2atB8ne200100Em ## -- Begin function _ZNSt3__15arrayIiLm5EE2atB8ne200100Em
	.globl	__ZNSt3__15arrayIiLm5EE2atB8ne200100Em
	.weak_definition	__ZNSt3__15arrayIiLm5EE2atB8ne200100Em
	.p2align	4, 0x90
__ZNSt3__15arrayIiLm5EE2atB8ne200100Em: ## @_ZNSt3__15arrayIiLm5EE2atB8ne200100Em
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)                 ## 8-byte Spill
	cmpq	$5, -16(%rbp)
	jb	LBB6_2
## %bb.1:
	leaq	L_.str.1(%rip), %rdi
	callq	__ZNSt3__120__throw_out_of_rangeB8ne200100EPKc
LBB6_2:
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	-16(%rbp), %rcx
	shlq	$2, %rcx
	addq	%rcx, %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.private_extern	__ZNSt3__120__throw_out_of_rangeB8ne200100EPKc ## -- Begin function _ZNSt3__120__throw_out_of_rangeB8ne200100EPKc
	.globl	__ZNSt3__120__throw_out_of_rangeB8ne200100EPKc
	.weak_definition	__ZNSt3__120__throw_out_of_rangeB8ne200100EPKc
	.p2align	4, 0x90
__ZNSt3__120__throw_out_of_rangeB8ne200100EPKc: ## @_ZNSt3__120__throw_out_of_rangeB8ne200100EPKc
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	$16, %edi
	callq	___cxa_allocate_exception
	movq	%rax, %rdi
	movq	%rdi, %rax
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	movq	-8(%rbp), %rsi
Ltmp0:
	callq	__ZNSt12out_of_rangeC1B8ne200100EPKc
Ltmp1:
	jmp	LBB7_1
LBB7_1:
	movq	-32(%rbp), %rdi                 ## 8-byte Reload
	movq	__ZTISt12out_of_range@GOTPCREL(%rip), %rsi
	movq	__ZNSt12out_of_rangeD1Ev@GOTPCREL(%rip), %rdx
	callq	___cxa_throw
LBB7_2:
Ltmp2:
	movq	-32(%rbp), %rdi                 ## 8-byte Reload
	movq	%rax, %rcx
	movl	%edx, %eax
	movq	%rcx, -16(%rbp)
	movl	%eax, -20(%rbp)
	callq	___cxa_free_exception
## %bb.3:
	movq	-16(%rbp), %rdi
	callq	__Unwind_Resume
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2, 0x0
GCC_except_table7:
Lexception0:
	.byte	255                             ## @LPStart Encoding = omit
	.byte	255                             ## @TType Encoding = omit
	.byte	1                               ## Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:
	.uleb128 Lfunc_begin0-Lfunc_begin0      ## >> Call Site 1 <<
	.uleb128 Ltmp0-Lfunc_begin0             ##   Call between Lfunc_begin0 and Ltmp0
	.byte	0                               ##     has no landing pad
	.byte	0                               ##   On action: cleanup
	.uleb128 Ltmp0-Lfunc_begin0             ## >> Call Site 2 <<
	.uleb128 Ltmp1-Ltmp0                    ##   Call between Ltmp0 and Ltmp1
	.uleb128 Ltmp2-Lfunc_begin0             ##     jumps to Ltmp2
	.byte	0                               ##   On action: cleanup
	.uleb128 Ltmp1-Lfunc_begin0             ## >> Call Site 3 <<
	.uleb128 Lfunc_end0-Ltmp1               ##   Call between Ltmp1 and Lfunc_end0
	.byte	0                               ##     has no landing pad
	.byte	0                               ##   On action: cleanup
Lcst_end0:
	.p2align	2, 0x0
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	__ZNSt12out_of_rangeC1B8ne200100EPKc ## -- Begin function _ZNSt12out_of_rangeC1B8ne200100EPKc
	.globl	__ZNSt12out_of_rangeC1B8ne200100EPKc
	.weak_def_can_be_hidden	__ZNSt12out_of_rangeC1B8ne200100EPKc
	.p2align	4, 0x90
__ZNSt12out_of_rangeC1B8ne200100EPKc:   ## @_ZNSt12out_of_rangeC1B8ne200100EPKc
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	__ZNSt12out_of_rangeC2B8ne200100EPKc
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.private_extern	__ZNSt12out_of_rangeC2B8ne200100EPKc ## -- Begin function _ZNSt12out_of_rangeC2B8ne200100EPKc
	.globl	__ZNSt12out_of_rangeC2B8ne200100EPKc
	.weak_def_can_be_hidden	__ZNSt12out_of_rangeC2B8ne200100EPKc
	.p2align	4, 0x90
__ZNSt12out_of_rangeC2B8ne200100EPKc:   ## @_ZNSt12out_of_rangeC2B8ne200100EPKc
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	%rdi, -24(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rsi
	callq	__ZNSt11logic_errorC2EPKc
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	__ZTVSt12out_of_range@GOTPCREL(%rip), %rcx
	addq	$16, %rcx
	movq	%rcx, (%rax)
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d\n"

L_.str.1:                               ## @.str.1
	.asciz	"array::at"

.subsections_via_symbols
