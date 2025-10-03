# Brief Assembly Refresher

Learn AT&T syntax

References:
- *1: https://danielggraham.com/wp-content/uploads/2019/08/assembly-refresher.pdf
- *2: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
- *3: https://www3.nd.edu/~dthain/compilerbook/chapter10.pdf

## Registers

Intel registers:

| name | 64  | 32  | Usage (*1)    | Architecture (*2)                         |
|------|-----|-----|---------------|-------------------------------------------|
| AX   | RAX | EAX | Return value  | Accumulator for operands and results data |
| BX   | RBX | EBX | Callee saved  | Pointer to data in the DS segment         |
| CX   | RCX | ECX | Argument #4   | Counter for string and loop operations    |
| DX   | RDX | EDX | Argument #3   | I/O pointer                               |
| SI   | RSI | ESI | Argument #2   | Source pointer for string operations      |
| DI   | RDI | EDI | Argument #1   | Destination pointer for string operations |
| SP   | RSP | ESP | Stack pointer | Stack pointer                             |
| BP   | RBP | EBP | Callee saved  | Pointer to data on the stack              |

Callee-saved registers are used to hold long-lied value that should be preserved across calls.

## AT&T syntax

```asm
movq $42, (%rbx)    # memory[rbx] <- 42
```

- destination last
  - `mov source, destination`
- *Registers* start with `%`. For example, `%rbx`.
- *Constants* start with `$`. For example, `$42` or `$0x2A`.
- `()`s represent value in memory. For example, `(%rbx)`.
- `q` in `movq` indicates length:
  - `movb`: 1 byte
  - `movw`: word or 2 bytes
  - `movl`: long or 4 bytes (2 words)
  - `movq`: quad or 8 bytes (4 words)

```asm
movq -8(%rbp), %rax           # %rax <- memory[rbp - 8]
movl %edx, (%rbx,%rcx,4)      # memory[rbx + (rcx * 4)] <- edx
movq $42, 10(%rbx,%rcx,4)     # memory[rbx + (rcx * 4) + 10] <- 42
```

- displace(base,index,scale) := base + index * scale + displace
  - displace defaults to 0
  - base defaults to 0
  - scale defaults to 1

| AT&T syntax         | memory address          | d  | b   | i   | s |
|:--------------------|:------------------------|---:|----:|----:|--:|
| `-8(%rbx)`          | `-8 + rbx `             | -8 | 0   | rbx | 1 |
| `(%rbx,%rcx,4)`     | `rbx + (rcx * 4)`       |  0 | rbx | rcx | 4 |
| `10(%rbx,%rcx,4)`   | `10 + rbx + (rcx * 4)`  | 10 | rbx | rcx | 4 |
| `10(%rcx,4)`        | `10 + (rcx * 4)`        | 10 | 0   | rcx | 4 |
| `10(,%rcx,4)`       | `10 + (rcx * 4)`        | 10 | 0   | rcx | 4 |
| `10(%rbx,%rcx)`     | `10 + rbx + rcx`        | 10 | rbx | tcx | 1 |

```asm
movq $0x0100, %rdx
movq $0x0004, %rcx
leaq (%rdx,%rcx,4), %rax    # %rax <- (0x0100 + (4 * 4)) = 0x0110
```

- `lea` computes the effective address of the source operand and stores it in the destination operand.

```asm
addl $1, %eax       # %eax <- %eax + 1
```

- `add` sets the OF and CF flags to indicate a carry (overflow) in the signed or unsigned result, respectively.


```asm
subq $4, %rsp       # %rsp <- %rsp - 4
```

- `sub` sets the OF and CF flags to indicate an overflow in the signed or unsigned result, respectively.

```asm
mull %ecx           # %edx:%eax <- %eax * %ecx
mulq %rcx           # %rdx:%rax <- %rax * %rcx
```

- `mul` performs an unsigned multiplication. RDX (EDX) contains the high-order bits of the product.
- If the high-order bits of the product are 0, then the CF and OF flags are cleared; otherwise, the flags are set.


```asm
divl %ecx           # %eax <- %edx:%eax / %ecx  (quotient)
                    # %edx <- %edx:%eax % %ecx  (reminder)
divq %rcx           # %rax <- %rdx:%rax / %rcx  (quotient)
                    # %rdx <- %rdx:%rax % %rcx  (reminder)
```

- `div` divides unsigned values in EDX:EAX (RDX:RAX) by the divisor (RCX above, but can be a memory location).
