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


        lui     $s0, BASE       #
        lw      $t1, TRISE($s0)         #       $t1 = [TRISE];
        andi    $t1, $t1, 0xFFF0        #       TRISE[3..0] = 0;
        sw      $t1, TRISE($s0)         #       WRITE TRISE Register
        li      $s1, 0                  #       count = 0;

        lw $t2,TRISB($s0)
        andi $t2,$t2,0x0004             #       TRISB[2] ou RB3 = 0;
        sw $t2,TRISB($s0)
        li $s1,0    #t3 = counter

whilecrescente:

        # if RB3 == 0 goto whiledecrescente 
        lw $t3,PORTB($s0)
        andi $t3,$t3,0x0004
        beq $t3,$0,whiledecrescente

        lw      $t1, LATE($s0)          #               $t1 = [LATE];
        andi    $t1, $t1, 0xFFF0        #               RE[3..0] = 0;
        or      $t1, $s1, $t1           #               RE[3..0] = count;
        sw      $t1, LATE($s0)          #               WRITE LATE Register;

        li $a0,100                      # FREQ = 1HZ
        jal delay

        addi $s1,$s1,1          #               count++;
        andi $s1,$s1,0x000F     #               count = count & 0x000F;
        j whilecrescente

whiledecrescente:
        #if RB3 == 1 goto whilecrescente
        lw $t3,PORTB($s0)
        andi $t3,$t3,0x0004
        bne $t3,$0,whilecrescente

        lw      $t1, LATE($s0)          #               $t1 = [LATE];
        andi    $t1, $t1, 0xFFF0        #               RE[3..0] = 0;
        or      $t1, $s1, $t1           #               RE[3..0] = count;
        sw      $t1, LATE($s0)          #               WRITE LATE Register;

        li $a0,250                     # FREQ = 4HZ    
        jal delay

        sub $s1,$s1,1          #               count--;
        andi $s1,$s1,0x000F     #               count = count & 0x000F;
        j whiledecrescente
END:
        lw      $ra, 0($sp)             #       repor $ra
        lw      $s0, 4($sp)             #       salvaguardar $s0
        lw      $s1, 8($sp)             #       salvaguardar $s1
        addiu   $sp, $sp, 12            #       repor espaco na stack
        jr      $ra 

        
###################

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
