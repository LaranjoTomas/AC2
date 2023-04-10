    .equ SFR_BASE_HI, 0xBF88
    .equ TRISE, 0x6100
    .equ PORTE, 0x6110
    .equ LATE, 0x6120
    .equ TRISB, 0x6040
    .equ PORTB, 0x6050                  #       0x6050 = 0110 0000 0101 0000
    .equ LATB, 0x6060
    .data
    .text
    .globl main

main:                                   #       int main(void) {
        lui     $t0, SFR_BASE_HI        #
        lw      $t1, TRISE($t0)         #       $t1 = [TRISE]
        andi    $t1, $t1, 0xFFF0        #       RE1 = 0 (OUTPUT) 1111 1111 1111 0000
        sw      $t1, TRISE($t0)         #       WRITE TRISE register
                                        
        lw      $t1, TRISB($t0)         #       $t1 = [TRISB]
        ori     $t1, $t1, 0x000F        #       RB0 = 1 (INPUT) 0000 0000 0000 1111
        sw      $t1, TRISB($t0)         #       WRITE TRISB register

loop:                                   #       while(1) {
        lw      $t1, PORTB($t0)         #               $t1 = [PORTB]
        andi    $t1, $t1, 0x000F        #               $t1 = RB03
        xori    $t1, $t1, 0x000F        #               usado para inverter o valor
        lw      $t2, LATE($t0)          #               $t2 = [LATE]
        andi    $t2, $t2, 0xFFF0        #               RE1 = 0
        or      $t2, $t2, $t1           #               RE1 = RB0
        sw      $t2, LATE($t0)          #
        j       loop                    #       }
        li      $v0, 0                  #       return 0;
        jr      $ra                     # }