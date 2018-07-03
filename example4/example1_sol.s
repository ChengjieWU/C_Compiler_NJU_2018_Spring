.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
ASSPRE___temp_variable_11__: .word 0
ASSPRE___temp_variable_10__: .word 0
ASSPRE___temp_variable_9__: .word 0
ASSPRE___temp_variable_8__: .word 0
ASSPRE___temp_variable_7__: .word 0
ASSPRE___temp_variable_6__: .word 0
ASSPRE___temp_variable_5__: .word 0
ASSPRE___temp_variable_4__: .word 0
ASSPRE___temp_variable_3__: .word 0
ASSPRE_c: .word 0
ASSPRE___temp_variable_2__: .word 0
ASSPRE___temp_variable_1__: .word 0
ASSPRE___temp_variable_0__: .word 0
ASSPRE_n: .word 0
ASSPRE_i: .word 0
ASSPRE_b: .word 0
ASSPRE_a: .word 0
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
main:
    li $t0, 0
    la $s0, ASSPRE_a
    sw $t0, 0($s0)
    li $t0, 1
    la $s0, ASSPRE_b
    sw $t0, 0($s0)
    li $t0, 0
    la $s0, ASSPRE_i
    sw $t0, 0($s0)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    la $s0, ASSPRE___temp_variable_0__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_0__
    lw $t0, 0($s0)
    la $s0, ASSPRE_n
    sw $t0, 0($s0)
__LABEL_0__:
    la $s0, ASSPRE_i
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_1__
    sw $t0, 0($s0)
    la $s0, ASSPRE_n
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_2__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_1__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_2__
    lw $t1, 0($s0)
    blt $t0, $t1, __LABEL_1__
    j __LABEL_2__
__LABEL_1__:
    la $s0, ASSPRE_a
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_3__
    sw $t0, 0($s0)
    la $s0, ASSPRE_b
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_4__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_3__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_4__
    lw $t1, 0($s0)
    add $t2, $t0, $t1
    la $s0, ASSPRE_c
    sw $t2, 0($s0)
    la $s0, ASSPRE_b
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_5__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_5__
    lw $t0, 0($s0)
    move $a0, $t0
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    la $s0, ASSPRE_b
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_6__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_6__
    lw $t0, 0($s0)
    la $s0, ASSPRE_a
    sw $t0, 0($s0)
    la $s0, ASSPRE_c
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_7__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_7__
    lw $t0, 0($s0)
    la $s0, ASSPRE_b
    sw $t0, 0($s0)
    la $s0, ASSPRE_i
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_9__
    sw $t0, 0($s0)
    li $t0, 1
    la $s0, ASSPRE___temp_variable_10__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_9__
    lw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_10__
    lw $t1, 0($s0)
    add $t2, $t0, $t1
    la $s0, ASSPRE___temp_variable_8__
    sw $t2, 0($s0)
    la $s0, ASSPRE___temp_variable_8__
    lw $t0, 0($s0)
    la $s0, ASSPRE_i
    sw $t0, 0($s0)
    j __LABEL_0__
__LABEL_2__:
    li $t0, 0
    la $s0, ASSPRE___temp_variable_11__
    sw $t0, 0($s0)
    la $s0, ASSPRE___temp_variable_11__
    lw $t0, 0($s0)
    move $v0, $t0
    jr $ra
