        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,6
        .data
        .text
        .globl main
main:   
        li $t0,0 # counter=0
while:  # while (1) {
        li $v0,RESET_CORE_TIMER #
        syscall # resetCoreTimer()
        move $t1,$v0

        li $v0,READ_CORE_TIMER
        syscall # readCoreTimer()
        li $t2,200000

        move $t3,$v0
        
        bge $t3,$t2,endwhile
        addi $t0,$t0,1
        move $a0,$t0
        li $t4,10
        or $a1,$t4,4
        sll $a1,$a1,4
        li $v0,PRINT_INT
        syscall

        li $a0,'r'
        li $v0,PUT_CHAR
        syscall
        
        li $a0,'\r'
        li $v0,PUT_CHAR
        syscall

        li $a0,20
        jal wait

        j while

endwhile:
        li $v0,0
        jr $ra



##############
wait:
    li $t3, 0

for:
    # multiply $v0 by 515000
    li $t4, 51500
    mult $a0, $t4
    mflo $t5
    
    addi $t3,$t3,1
    bgt $t3, $t5, endfor
    j for

endfor:
    jr $ra