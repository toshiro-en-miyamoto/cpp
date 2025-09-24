```
	movl	$0, -4(%rbp)          // int main()
	movl	$0, -8(%rbp)          // int result
	movl	$0, -12(%rbp)         // int i
LBB0_1:
	movslq	-12(%rbp), %rax
	cmpq	$40, %rax             // i < N ?
	jae	LBB0_4                  // if false, exit the loop
## %bb.2:
	movslq	-12(%rbp), %rcx
	leaq	__ZL6buffer(%rip), %rax
	movl	(%rax,%rcx,4), %eax   // (eax) = buffer[i]
	addl	-8(%rbp), %eax        // (eax) = result + buffer[i]
	movl	%eax, -8(%rbp)        // result = (eax)
## %bb.3:
	movl	-12(%rbp), %eax
	addl	$1, %eax                // i++
	movl	%eax, -12(%rbp)
	jmp	LBB0_1
LBB0_4:
	movl	-4(%rbp), %eax
	popq	%rbp
	retq
```