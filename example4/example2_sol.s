.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
ASSPRE___temp_variable_16__: .word 0
ASSPRE___temp_variable_15__: .word 0
ASSPRE___temp_variable_14__: .word 0
ASSPRE___temp_variable_13__: .word 0
ASSPRE___temp_variable_12__: .word 0
ASSPRE___temp_variable_11__: .word 0
ASSPRE___temp_variable_10__: .word 0
ASSPRE___temp_variable_9__: .word 0
ASSPRE_result: .word 0
ASSPRE_m: .word 0
ASSPRE___temp_variable_8__: .word 0
ASSPRE___temp_variable_7__: .word 0
ASSPRE___temp_variable_6__: .word 0
ASSPRE___temp_variable_5__: .word 0
ASSPRE___temp_variable_4__: .word 0
ASSPRE___temp_variable_3__: .word 0
ASSPRE___temp_variable_2__: .word 0
ASSPRE___temp_variable_1__: .word 0
ASSPRE___temp_variable_0__: .word 0
ASSPRE_n: .word 0
.globl main
.text
read:
    li $v0, 4
    la $a0, _prompt
    syscall
    li $v0, 5
    syscall
    jr $ra
write:
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, _ret
    syscall
    move $v0, $0
    jr $ra
fact:
    move $t0, $a0
    la $s0, ASSPRE_n
    sw $t0, 0($s0)
    la $s0, ASSPRE_n
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_0__
    sw $t0, 0($s0)
    li $t0, 1
    la $s0, ASSPRE___temp_variable_1__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_0__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_1__
    lw $t1, 0($s0)
    beq $t0, $t1, __LABEL_0__
    j __LABEL_1__
__LABEL_0__:
    la $s0, ASSPRE_n
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_2__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_2__
    lw $t0, 0($s0)
    move $v0, $t0
    jr $ra
    j __LABEL_2__
__LABEL_1__:
    la $s0, ASSPRE_n
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_4__
    sw $t0, 0($s0)
    la $s0, ASSPRE_n
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_7__
    sw $t0, 0($s0)
    li $t0, 1
    la $s0, ASSPRE___temp_variable_8__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_7__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_8__
    lw $t1, 0($s0)
    sub $t2, $t0, $t1
    la $s0, ASSPRE___temp_variable_6__
    sw $t2, 0($s0)
    la $s0, ASSPRE___temp_variable_6__
    lw $t0, 0($s0)
    move $a0, $t0
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fact
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    la $s0, ASSPRE___temp_variable_5__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_4__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_5__
    lw $t1, 0($s0)
    mul $t2, $t0, $t1
    la $s0, ASSPRE___temp_variable_3__
    sw $t2, 0($s0)
    la $s0, ASSPRE___temp_variable_3__
    lw $t0, 0($s0)
    move $v0, $t0
    jr $ra
__LABEL_2__:
main:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    la $s0, ASSPRE___temp_variable_9__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_9__
    lw $t0, 0($s0)
    la $s0, ASSPRE_m
    sw $t0, 0($s0)
    la $s0, ASSPRE_m
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_10__
    sw $t0, 0($s0)
    li $t0, 1
    la $s0, ASSPRE___temp_variable_11__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_10__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_11__
    lw $t1, 0($s0)
    bgt $t0, $t1, __LABEL_3__
    j __LABEL_4__
__LABEL_3__:
    la $s0, ASSPRE_m
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_13__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_13__
    lw $t0, 0($s0)
    move $a0, $t0
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fact
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    la $s0, ASSPRE___temp_variable_12__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_12__
    lw $t0, 0($s0)
    la $s0, ASSPRE_result
    sw $t0, 0($s0)
    j __LABEL_5__
__LABEL_4__:
    li $t0, 1
    la $s0, ASSPRE___temp_variable_14__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_14__
    lw $t0, 0($s0)
    la $s0, ASSPRE_result
    sw $t0, 0($s0)
__LABEL_5__:
    la $s0, ASSPRE_result
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_15__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_15__
    lw $t0, 0($s0)
    move $a0, $t0
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    li $t0, 0
    la $s0, ASSPRE___temp_variable_16__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_16__
    lw $t0, 0($s0)
    move $v0, $t0
    jr $ra
