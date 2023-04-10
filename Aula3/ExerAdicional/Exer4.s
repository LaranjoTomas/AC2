        .equ    BASE, 0xBF88
        .equ    TRISE, 0x6100
        .equ    PORTE, 0x6110
        .equ    LATE, 0x6120
        .equ    TRISB, 0x6040
        .equ    TRISD, 0x60C0
        .equ    PORTB, 0x6050
        .equ    PORTD, 0x60D0
        .equ    LATB, 0x6060
        .equ    LATD, 0x60E0
        .equ    RESET_CORE_TIMER,12
        .equ    READ_CORE_TIMER,11
        .data
        .text

        .global main
    
main:
        lui $s0,BASE
        lw $t0,TRISE($s0)
        andi $t0,$t0,0xFFFE
        sw $t0,TRISE($s0)

        lw $t0,TRISD($s0)
        andi $t0,$t0,0xFFFE
        sw $t0,TRISD($s0)


        li $s1,0                #v =0

        
loop:
        lw $t0,LATE($s0)
        andi $t0,$t0,0xFFFE
        or $t0,$t0,$s1
        sw $t0,LATE($s0)

        lw $t0,LATD($s0)
        andi $t0,$t0,0xFFFE
        or $t0,$t0,$s1
        sw $t0,LATD($s0)


        li $a0,500
        jal delay

        xor $s1,$s1,1
        j loop

end:
        li $v0,0
        jr $ra
###############
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

