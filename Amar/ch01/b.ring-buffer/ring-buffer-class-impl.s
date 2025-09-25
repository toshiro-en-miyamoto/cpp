	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 0
	.globl	__ZN11ring_buffer4pushEi        ## -- Begin function _ZN11ring_buffer4pushEi
	.p2align	4, 0x90
__ZN11ring_buffer4pushEi:               ## @_ZN11ring_buffer4pushEi
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
	jae	LBB0_2
## %bb.1:
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	40(%rax), %rcx
	addq	$1, %rcx
	movq	%rcx, 40(%rax)
	jmp	LBB0_3
LBB0_2:
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	32(%rax), %rax
	addq	$1, %rax
	movl	$5, %ecx
	xorl	%edx, %edx
                                        ## kill: def $rdx killed $edx
	divq	%rcx
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	%rdx, 32(%rax)
LBB0_3:
	addq	$32, %rsp
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
	jb	LBB1_2
## %bb.1:
	leaq	L_.str(%rip), %rdi
	callq	__ZNSt3__120__throw_out_of_rangeB8ne200100EPKc
LBB1_2:
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	-16(%rbp), %rcx
	shlq	$2, %rcx
	addq	%rcx, %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZN11ring_buffer3popEv         ## -- Begin function _ZN11ring_buffer3popEv
	.p2align	4, 0x90
__ZN11ring_buffer3popEv:                ## @_ZN11ring_buffer3popEv
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
	jne	LBB2_2
## %bb.1:
	movl	$0, -4(%rbp)
	jmp	LBB2_3
LBB2_2:
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
LBB2_3:
	movl	-4(%rbp), %eax
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
	jmp	LBB3_1
LBB3_1:
	movq	-32(%rbp), %rdi                 ## 8-byte Reload
	movq	__ZTISt12out_of_range@GOTPCREL(%rip), %rsi
	movq	__ZNSt12out_of_rangeD1Ev@GOTPCREL(%rip), %rdx
	callq	___cxa_throw
LBB3_2:
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
GCC_except_table3:
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
	.asciz	"array::at"

.subsections_via_symbols
