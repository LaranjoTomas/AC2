        .equ    printInt,6
        .equ    putChar,3
        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ inkey, 1
        .data
a:    .asciiz "A"
n:    .asciiz "N"
        .text
        .globl main
main:
        li $t0,0    #t0= cnt1
        li $t1,0    #t1= cnt5
        li $t2,0    #t2= cnt10
        li $t3,0    #t3= key
do:
        li $v0,inkey
        syscall
        move $t3,$v0
        
        la $t4,a
        bne $t3,'a',if2

if1:    addi $t0,$t0,1  #cnt1++
        li $a0,'\r'
        li $v0,putChar
        syscall

        move $a0,$t0
        li $v0,printInt
        syscall

        li $a0,' '
        li $v0,putChar
        syscall

        addi $t1,$t1,5  #cnt5++

        move $a0,$t1
        li $v0,printInt
        syscall

        li $a0,' '
        li $v0,putChar
        syscall

        addi $t2,$t2,10  #cnt10++

        move $a0,$t2
        li $v0,printInt
        syscall

        li $a0,' '
        li $v0,putChar
        syscall

        li $a0,1000
        jal delay

        j do
do2:
        li $v0,inkey
        syscall
        move $t3,$v0
        
        la $t4,n
        beq $t3,'n',if1

if2:    addi $t0,$t0,2  #cnt1++
        li $a0,'\r'
        li $v0,putChar
        syscall

        move $a0,$t0
        li $v0,printInt
        syscall

        li $a0,' '
        li $v0,putChar
        syscall

        addi $t1,$t1,10  #cnt5++

        move $a0,$t1
        li $v0,printInt
        syscall

        li $a0,' '
        li $v0,putChar
        syscall

        addi $t2,$t2,20  #cnt10++

        move $a0,$t2
        li $v0,printInt
        syscall

        li $a0,' '
        li $v0,putChar
        syscall

        li $a0,1000
        jal delay

        j do2


#####################################
delay:

        la $v0,RESET_CORE_TIMER
        syscall # readCoreTimer()

WHILE: 
        la $v0,READ_CORE_TIMER
        syscall # readCoreTimer()
        li $t5,20000
        mult $t5,$a0
        mflo $t5
        bge $v0,$t5,ENDWHILE

        j WHILE

ENDWHILE:
        jr $ra