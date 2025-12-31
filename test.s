.globl main
.ascii "Hello, World!"
.section text

main:
    ret
    mov 0xFFFF, %rax