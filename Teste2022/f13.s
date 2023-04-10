#Faça um programa em mips assemble que nos 4 leds (RE4-RE1) o padrão 1001 , 0110, 1001,0110,... está sempre a alterar-se com uma frequência de 7Hz.
#freq=7hz print padrão para o ecra em binário 4 bits
# 1001 = 9   0110 = 6
        .equ printint,6
        .equ putChar,3
        .equ BASE, 0xBF88
        .equ TRISE, 0x6100
        .equ LATE, 0x6120
        .equ RESET_CORE_TIMER,12
        .equ READ_CORE_TIMER,11
        .data
        .text
        .globl main
    
main:
        lui     $t0, BASE
        
        lw      $t1, TRISE($t0)
        andi    $t1, $t1, 0xFFF0      #1111 1111 1111 0000
        sw      $t1, TRISE($t0)

loop:

        lw      $t1, LATE($t0)
        andi    $t1, $t1, 0xFFF0
        ori     $t1, $t1, 0x0009
        sw      $t1, LATE($t0)

        li $a0,0x9
        li $a1,2
        li $v0,printint
        syscall

        li $a0,1000
        jal delay

        lw      $t1, LATE($t0)
        andi    $t1, $t1, 0xFFF0
        ori     $t1, $t1, 0x0006
        sw      $t1, LATE($t0)

        li $a0,0x6
        li $a1,2
        li $v0,printint
        syscall

        li $a0,1000
        jal delay
        
        j loop

endloop:
        li $v0,0
        jr $ra



##############################

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
