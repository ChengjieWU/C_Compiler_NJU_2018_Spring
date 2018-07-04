.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
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
    addi $sp, $sp, -4
    sw $fp, 0($sp)
    move $fp, $sp
    addi $sp, $sp, -80
    lw $t0, 8($fp)
    move $t1, $t0
    li $t2, 1
    move $t3, $t2
    sw $t0, -4($fp)
    sw $t1, -8($fp)
    sw $t3, -12($fp)
    beq $t1, $t3, __LABEL_0__
    j __LABEL_1__
__LABEL_0__:
    lw $t4, -4($fp)
    move $t5, $t4
    move $v0, $t5
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    sw $t5, -16($fp)
    j __LABEL_2__
__LABEL_1__:
    lw $t6, -4($fp)
    move $t7, $t6
    move $t8, $t6
    li $t9, 1
    move $t0, $t9
    sub $t1, $t8, $t0
    addi $sp, $sp, -4
    sw $t1, 0($sp)
    sw $t0, -40($fp)
    sw $t1, -32($fp)
    sw $t7, -24($fp)
    sw $t8, -36($fp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fact
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t2, $v0
    addi $sp, $fp, -80
    lw $t3, -24($fp)
    mul $t4, $t3, $t2
    move $v0, $t4
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    sw $t2, -28($fp)
    sw $t4, -20($fp)
__LABEL_2__:
main:
    addi $sp, $sp, -4
    sw $fp, 0($sp)
    move $fp, $sp
    addi $sp, $sp, -80
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t5, $v0
    move $t6, $t5
    move $t7, $t6
    li $t8, 1
    move $t9, $t8
    sw $t5, -52($fp)
    sw $t6, -44($fp)
    sw $t7, -56($fp)
    sw $t9, -60($fp)
    bgt $t7, $t9, __LABEL_3__
    j __LABEL_4__
__LABEL_3__:
    lw $t0, -44($fp)
    move $t1, $t0
    addi $sp, $sp, -4
    sw $t1, 0($sp)
    sw $t1, -68($fp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fact
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t2, $v0
    addi $sp, $fp, -80
    move $t3, $t2
    sw $t2, -64($fp)
    sw $t3, -48($fp)
    j __LABEL_5__
__LABEL_4__:
    li $t4, 1
    move $t5, $t4
    move $t6, $t5
    sw $t5, -72($fp)
    sw $t6, -48($fp)
__LABEL_5__:
    lw $t7, -48($fp)
    move $t8, $t7
    move $a0, $t8
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    li $t9, 0
    move $t0, $t9
    move $v0, $t0
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
