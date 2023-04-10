        .equ    printStr, 8
        .equ    readStr, 9
        .equ    printInt, 6
        .equ    printInt10, 5
        .equ    SIZE, 20
        .data
s1:     .asciiz "Introduza 2 strings: "
s2:     .asciiz "Resultados:\n"
str1:   .space  21
str2:   .space  21
str3:   .space  41
        .text
        .globl main
    
main:
    addiu   $sp, $sp, -4        #
    sw      $ra, 0($sp)         #

    la      $a0, s1             #
    li      $v0, printStr       #
    syscall                     # printStr("Introduza 2 strings: ");

    la    $a0, str1             #
    li    $a1, SIZE             #
    li    $v0, readStr          #
    syscall                     # readStr( str1, SIZE );

    la    $a0, str2             #
    li    $a1, SIZE             #
    li    $v0, readStr          #
    syscall                     # readStr( str2, SIZE );

    la      $a0, s2             #
    li      $v0, printStr       #
    syscall                     # printStr("Resultados:\n");

    la    $a0, str1           #
    jal     strlen              # strlen(str1)
    move    $a0, $v0            #
    li      $a1, 10             #
    li      $v0, printInt       #
    syscall                     # printInt( strlen(str1), 10 );

    la    $a0, str2           #
    jal     strlen              # strlen(str2)
    move    $a0, $v0            #
    li      $a1, 10             #
    li      $v0, printInt       #
    syscall                     # printInt( strlen(str2), 10 );

    la    $a0, str3           #
    la    $a1, str1           #
    jal     strcpy              # strcpy(str3, str1);

    la    $a0, str3           #
    la    $a1, str2           #
    jal     strcat              # strcat(str3, str2)
    move    $a0, $v0            #
    li      $v0, printStr       #
    syscall                     # printStr( strcat(str3, str2) );

    la    $a0, str1           #
    la    $a1, str2           #
    jal     strcmp              # strcmp(str1, str2)
    move    $a0, $v0            #
    li      $v0, printStr       #
    syscall                     # printStr( strcmp(str1, str2) );

    li      $v0, 0              # return 0;

    lw      $ra, 0($sp)         #
    addiu   $sp, $sp, 4         #
    jr      $ra                 # end of program


strlen:                                 # int strlen(char *s) {
        move    $t0, $a0                #       $t0 = &s
        li      $v0, 0                  #       len = 0;
for:    lb      $t1, 0($t0)             #       $t1 = *s
        beq     $t1, $0, endfor       	#       for(len = 0; *s != '\0'; len++, s++) {
        addiu   $t0, $t0, 1             #               $t0++;   //next position of string
        addi    $v0, $v0, 1             #               len++;
        j       for                     #       }
endfor:                                 #       return len;
        jr      $ra                     # }


strcpy:                                 # char *strcpy(char *dst, char *src) {
        move    $v0, $a0                #       char *p = dst;
for2:   lb      $t1, 0($a1)             #       $t1 = *src;
        sb      $t1, 0($a0)             #               *dst = *src;
        beq     $t1, $0, endfor2      	#       for(; *src = '\0'; dst++, src++) { ###mbc troquei esta instrução 
        addiu   $a0, $a0, 1             #               dst++;
        addiu   $a1, $a1, 1             #               src++;
        j       for2                    #       }
endfor2:                                #       return p;
        jr      $ra                     # }


strcat:                                 # char *strcat(char *dst, char *src) {
        addiu   $sp, $sp, -8            #       abrir espaco na pilha
        sw      $s0, 0($sp)             #       salvaguardar $s0
        sw      $ra, 4($sp)             #       salvaguardar $ra
        move    $s0, $a0                #       $s0 = dst;

for3:   lb      $t0, 0($a0)             #       for (; *dst != '\0'; dst++) {
        beq     $t0, $0, endfor3      	#
        addiu   $a0, $a0, 1             #               dst++;
        j       for3                    #       }
endfor3:                                #       
        jal     strcpy                  #       strcpy(dst, src)
        move    $v0, $s0                #       return p;
        lw      $s0, 0($sp)             #       repor $s0
        lw      $ra, 4($sp)             #       repor $ra
        addiu   $sp, $sp, 8             #       repor tamanho da pilha
        jr      $ra                     # }


# Returned value is:      
#   < 0  string "s1" is "less than" string "s2"      
#   = 0  string "s1" is equal to string "s2"      
#   > 0  string "s1" is "greater than" string "s2"
strcmp:                                 # int strcmp(char *s1, char *s2) {
for4:   lb      $t0, 0($a0)             #       $t0 = *s1      
        lb      $t1, 0($a1)             #       $t1 = *s2
        bne     $t0, $t1, endfor4       #       for( ;(*s1 == *s2)  &&
        beq     $t0, $0, endfor4        #       (*s1 != '\0'); s1++, s2++) {
        addiu   $a0, $a0, 1             #               s1++;
        addiu   $a1, $a1, 1             #               s2++;
        j       for4                    #       }
endfor4:                                #
        sub     $v0, $t0, $t1           #       return (*s1 - *s2);
        jr      $ra                     # }
