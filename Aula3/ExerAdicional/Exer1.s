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
    .data
    .text

    .global main

main:
    lui $t1, BASE # $t1=0xBF880000
    lw $t2, TRISE($t1) # 
    andi $t2, $t2, 0xFFF0 # MODIFY (bit0=bit3=0 (0 means OUTPUT))
    sw $t2, TRISE($t1) # WRITE (Write TRISE register)

    lw $t3, TRISB($t1)
    andi $t3,$t3,0x000F
    sw $t3, TRISB($t1)

loop:

    lw $t4, PORTE($t1)
    andi $t4, $t4, 0x000F
    sw $t4, LATE($t1)

    j loop

end:
    li $v0,0
    jr $ra
