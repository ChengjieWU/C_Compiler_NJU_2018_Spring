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
    sw $t0, -76($fp)
    lw $t0, -76($fp)
    sw $t0, -72($fp)
    li $t0, 1
    sw $t0, -68($fp)
    lw $t0, -72($fp)
    lw $t1, -68($fp)
    beq $t0, $t1, __LABEL_0__
    j __LABEL_1__
__LABEL_0__:
    lw $t0, -76($fp)
    sw $t0, -64($fp)
    lw $t0, -64($fp)
    move $v0, $t0
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    j __LABEL_2__
__LABEL_1__:
    lw $t0, -76($fp)
    sw $t0, -56($fp)
    lw $t0, -76($fp)
    sw $t0, -44($fp)
    li $t0, 1
    sw $t0, -40($fp)
    lw $t0, -44($fp)
    lw $t1, -40($fp)
    sub $t2, $t0, $t1
    sw $t2, -48($fp)
    lw $t0, -48($fp)
    addi $sp, $sp, -4
    sw $t0, 0($sp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fact
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    sw $t0, -52($fp)
    addi $sp, $fp, -80
    lw $t0, -56($fp)
    lw $t1, -52($fp)
    mul $t2, $t0, $t1
    sw $t2, -60($fp)
    lw $t0, -60($fp)
    move $v0, $t0
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
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
    move $t0, $v0
    sw $t0, -28($fp)
    lw $t0, -28($fp)
    sw $t0, -36($fp)
    lw $t0, -36($fp)
    sw $t0, -24($fp)
    li $t0, 1
    sw $t0, -20($fp)
    lw $t0, -24($fp)
    lw $t1, -20($fp)
    bgt $t0, $t1, __LABEL_3__
    j __LABEL_4__
__LABEL_3__:
    lw $t0, -36($fp)
    sw $t0, -12($fp)
    lw $t0, -12($fp)
    addi $sp, $sp, -4
    sw $t0, 0($sp)
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal fact
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    move $t0, $v0
    sw $t0, -16($fp)
    addi $sp, $fp, -80
    lw $t0, -16($fp)
    sw $t0, -32($fp)
    j __LABEL_5__
__LABEL_4__:
    li $t0, 1
    sw $t0, -8($fp)
    lw $t0, -8($fp)
    sw $t0, -32($fp)
__LABEL_5__:
    lw $t0, -32($fp)
    sw $t0, -4($fp)
    lw $t0, -4($fp)
    move $a0, $t0
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal write
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    li $t0, 0
    sw $t0, -0($fp)
    lw $t0, -0($fp)
    move $v0, $t0
    move $sp, $fp
    lw $fp, 0($sp)
    addi $sp, $sp, 4
    jr $ra
