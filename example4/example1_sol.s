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
    sw $t0, -64($fp)
    li $t0, 1
    sw $t0, -60($fp)
    li $t0, 0
    sw $t0, -56($fp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal read
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    sw $t0, -48($fp)
    lw $t0, -48($fp)
    sw $t0, -52($fp)
__LABEL_0__:
    lw $t0, -56($fp)
    sw $t0, -44($fp)
    lw $t0, -52($fp)
    sw $t0, -40($fp)
    lw $t0, -44($fp)
    lw $t1, -40($fp)
    blt $t0, $t1, __LABEL_1__
    j __LABEL_2__
__LABEL_1__:
    lw $t0, -64($fp)
    sw $t0, -32($fp)
    lw $t0, -60($fp)
    sw $t0, -28($fp)
    lw $t0, -32($fp)
    lw $t1, -28($fp)
    add $t2, $t0, $t1
    sw $t2, -36($fp)
    lw $t0, -60($fp)
    sw $t0, -24($fp)
    lw $t0, -24($fp)
    move $a0, $t0
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    lw $t0, -60($fp)
    sw $t0, -20($fp)
    lw $t0, -20($fp)
    sw $t0, -64($fp)
    lw $t0, -36($fp)
    sw $t0, -16($fp)
    lw $t0, -16($fp)
    sw $t0, -60($fp)
    lw $t0, -56($fp)
    sw $t0, -8($fp)
    li $t0, 1
    sw $t0, -4($fp)
    lw $t0, -8($fp)
    lw $t1, -4($fp)
    add $t2, $t0, $t1
    sw $t2, -12($fp)
    lw $t0, -12($fp)
    sw $t0, -56($fp)
    j __LABEL_0__
__LABEL_2__:
    li $t0, 0
    sw $t0, -0($fp)
    lw $t0, -0($fp)
    move $v0, $t0
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
