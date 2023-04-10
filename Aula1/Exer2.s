        .equ Print_str, 8

        .data
msg:    .asciz "AC2 - Aulas praticas\n"
        .text
        .globl main

main:
        la $a0, msg
        li $v0, Print_str
        syscall

        li $v0,0
        jr $ra