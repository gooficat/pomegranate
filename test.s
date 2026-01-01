.globl main
.ascii "Hello, World!"
.section text

main:
    mov [$main + 67], %rax
    ret
