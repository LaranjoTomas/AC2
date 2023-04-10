        .equ getChar,2
        .equ putChar,3
        .data
        .text
        .globl main

main:
        li $v0,0

do:
        li $v0,getChar
        syscall
        move $t0,$v0
        move $t1,$v0
        add $t0,$t0,1

        move $a0,$t0
        li $v0,putChar
        syscall

        beq $t1,'\n',end
        j do

end:    li $v0,0
        jr $ra
