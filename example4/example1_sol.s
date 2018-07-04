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
main:
    addi $sp, $sp, -4
    sw $fp, 0($sp)
    move $fp, $sp
    addi $sp, $sp, -68
    li $t0, 0
    move $t1, $t0
    li $t2, 1
    move $t3, $t2
    li $t4, 0
    move $t5, $t4
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t6, $v0
    move $t7, $t6
    sw $t1, -64($fp)
    sw $t3, -60($fp)
    sw $t5, -56($fp)
    sw $t6, -48($fp)
    sw $t7, -52($fp)
__LABEL_0__:
    lw $t8, -56($fp)
    move $t9, $t8
    lw $t0, -52($fp)
    move $t1, $t0
    sw $t1, -40($fp)
    sw $t9, -44($fp)
    blt $t9, $t1, __LABEL_1__
    j __LABEL_2__
__LABEL_1__:
    lw $t2, -64($fp)
    move $t3, $t2
    lw $t4, -60($fp)
    move $t5, $t4
    add $t6, $t3, $t5
    move $t7, $t4
    move $a0, $t7
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t8, $t4
    move $t2, $t8
    move $t9, $t6
    move $t4, $t9
    lw $t0, -56($fp)
    move $t1, $t0
    sw $t2, -64($fp)
    li $t2, 1
    sw $t3, -32($fp)
    move $t3, $t2
    sw $t4, -60($fp)
    add $t4, $t1, $t3
    move $t0, $t4
    sw $t0, -56($fp)
    sw $t1, -8($fp)
    sw $t3, -4($fp)
    sw $t4, -12($fp)
    sw $t5, -28($fp)
    sw $t6, -36($fp)
    sw $t7, -24($fp)
    sw $t8, -20($fp)
    sw $t9, -16($fp)
    j __LABEL_0__
__LABEL_2__:
    li $t5, 0
    move $t6, $t5
    move $v0, $t6
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
