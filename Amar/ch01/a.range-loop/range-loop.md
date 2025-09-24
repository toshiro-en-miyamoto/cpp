```
	movl	$0, -4(%rbp)        // int main()
	movl	$0, -8(%rbp)        // int result
	leaq	__ZL6buffer(%rip), %rax
	movq	%rax, -16(%rbp)     // begin(buffer)
	leaq	__ZL6buffer(%rip), %rax
	movq	%rax, -24(%rbp)     // int* ptr
	leaq	__ZL6buffer(%rip), %rax
	addq	$40, %rax
	movq	%rax, -32(%rbp)     // end(buffer)
LBB0_1:
	movq	-24(%rbp), %rax
	cmpq	-32(%rbp), %rax     // if ptr == end(buffer),
	je	LBB0_4                // then exit the loop
## %bb.2:
	movq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax        // (eax) = buffer[i]
	addl	-8(%rbp), %eax      // (eax) = result + buffer[i] 
	movl	%eax, -8(%rbp)      // result = result + buffer[i]
## %bb.3:
	movq	-24(%rbp), %rax
	addq	$4, %rax
	movq	%rax, -24(%rbp)     // ptr++
	jmp	LBB0_1
LBB0_4:
	movl	-4(%rbp), %eax
	popq	%rbp
	retq
```