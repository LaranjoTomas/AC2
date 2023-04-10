        .equ UP, 1
        .equ DOWN, 0
        .equ putChar,3
        .equ printInt,6
        .equ inkey,1
        .data
        .text
        .globl main

main:
        li $t0,0        #state = 0
        li $t1,0        # cnt = 0
        li $t2,0        # c

do:
        la $a0, 'r'
        li $v0, putChar
        syscall

        li $t4,10
        or $t3,$t4,3
        sll $t3, $t3, 4

        move $a0,$t1
        move $a1,$t3
        li $v0,printInt
        syscall

        la $a0,'\t'
        li $v0,putChar
        syscall

        li $t4,2
        or $t3,$t2,8
        sll $t3, $t3, 4

        move $a0,$t1
        move $a1,$t3
        li $v0,printInt
        syscall

        li $a0,5
        jal wait

        li $v0,inkey
        syscall
        move $t2,$v0

if:
        bne $t2, '+', if2
        la $t0, UP

if2:
        bne $t2, '-', if3
        la $t0, DOWN

if3:
        bne $t0, UP, else
        addi $t1, $t1, 1
        and  $t1,$t1,0xFF 
        j enddo
else:
        addi $t1, $t1, -1
        and  $t1,$t1,0xFF
        
enddo:
        beq $t2, 'q', end
        j do

end:
        li $v0,0
        jr $ra

###################################

wait:
    li $t3, 0

for:
    # multiply $v0 by 515000
    li $t4, 515000
    mult $a0, $t4
    mflo $t5
    
    addi $t3,$t3,1
    bgt $t3, $t5, endfor
    j for

endfor:
    jr $ra