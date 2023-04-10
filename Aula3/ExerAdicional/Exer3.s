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
        addiu   $sp, $sp, -12           #       abrir espaco na stack
        sw      $ra, 0($sp)             #       salvaguardar $ra
        sw      $s0, 4($sp)             #       salvaguardar $s0
        sw      $s1, 8($sp)             #       salvaguardar $s1

        lui    $s0,BASE
        lw     $t1,TRISE($s0)
        addi    $t1,$t1,0XFFC3            # RE2-RE5 -- 1111 1111 1100 0011
        sw     $t1,TRISE($s0)

        lw      $t1,TRISB($s0)
        addi    $t1,$t1,0X000F            # RB0-RB3 -- 0000 0000 0000 1111
        sw      $t1,TRISB($s0)

while:
        #RE2 = RB0\, RE3 = RB1, RE4 = RB2 e RE5 = RB3\
        lw      $t1,PORTB($s0)
        srl     $t1,$t1,1           # shift right to get RB0
        andi    $t1,$t1,1           # mask out all bits except RB0
        sll     $t1,$t1,2           # shift left to position RB0 on bit 2
        lw      $t2,LATE($s0)
        andi    $t2,$t2,0xFFF0      # mask out RE2-RE5 bits
        or      $t2,$t2,$t1         # set RE2 bit with RB0 value
        sw      $t2,LATE($s0)

        lw      $t1,PORTB($s0)
        srl     $t1,$t1,2           # shift right to get RB1
        andi    $t1,$t1,1           # mask out all bits except RB1
        sll     $t1,$t1,6           # shift left to position RB1 on bit 3
        lw      $t2,LATE($s0)
        andi    $t2,$t2,0xFFCF      # mask out RE3 bit
        or      $t2,$t2,$t1         # set RE3 bit with RB1 value
        sw      $t2,LATE($s0)

        lw      $t1,PORTB($s0)
        srl     $t1,$t1,3           # shift right to get RB2
        andi    $t1,$t1,1           # mask out all bits except RB2
        sll     $t1,$t1,4           # shift left to position RB2 on bit 4
        lw      $t2,LATE($s0)

        andi    $t2,$t2,0xFF3F      # mask out RE4 bit
        or      $t2,$t2,$t1         # set RE4 bit with RB2 value
        sw      $t2,LATE($s0)

        lw      $t1,PORTB($s0)
        srl     $t1,$t1,4           # shift right to get RB3
        andi    $t1,$t1,1           # mask out all bits except RB3
        sll     $t1,$t1,5           # shift left to position RB3 on bit 5
        lw      $t2,LATE($s0)
        andi    $t2,$t2,0xFEFF      # mask out RE5 bit
        or      $t2,$t2,$t1         # set RE5 bit with RB3 value
        sw      $t2,LATE($s0)


        j while

END:
        lw      $ra, 0($sp)             #       repor $ra
        lw      $s0, 4($sp)             #       salvaguardar $s0
        lw      $s1, 8($sp)             #       salvaguardar $s1
        addiu   $sp, $sp, 12            #       repor espaco na stack
        jr      $ra 
#############################3
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
