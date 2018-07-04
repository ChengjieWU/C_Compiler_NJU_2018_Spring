#include "codeGeneration.h"

static int GEN_REGS = 10;

extern struct Symbol_t *sym_table;
int params_count = 0;
int params_iter = 0;
int space_size = 0;
int reg_scan = 0;

/* 针对寄存器的动态分配 */
/*
用 FUNCTION 和 LABEl 作为点来划分基本块，基本块之间的转移可以通过
顺序执行、goto 和条件跳转、函数调用和返回。
注意，这边有一点很搞的。既要保持编译器本身数据结构，记录哪些是 free
哪些不是，也要考虑目标程序执行过程的情况。这两个是很很搞的。
由于基本块之间的执行顺序不确定，因此寄存器的分配必须是在基本块内部的。
call 语句跳转前要 dump 一下，return 时由于子函数的局部变量都会丢弃，
所以不需要再 dump 回去，只要是元控制上 regs 都是 free 即可。
FUNCTION 表示一个函数开始了，将元控制赋为 free。（绝对不能少）
GOTO 和条件跳转执行前要将寄存器 dump 回去。
顺序进入 LABEL 前要把寄存器 dump 回去。
*/


void generate_text(CB ic, FILE *f)
{
    int count = 0;
    struct InterCode *code = ic.begin;
    for (; code != NULL; code = code->next) {
        int r0, r1, r2, i;
        switch (code->kind) {
            case IR_LABEL:
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "%s:\n", code->op1.sym->name);
                break;
            case IR_FUNCTION:
                for (i = 0; i < MAX_REGS; i++) {
                     regs[i].free = true;
                }
                fprintf(f, "%s:\n", code->op1.sym->name);
                params_count = code->op1.sym->num_params;
                params_iter = 0;
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $fp, 0($sp)\n");
                fprintf(f, "    move $fp, $sp\n");
                fprintf(f, "    addi $sp, $sp, -%d\n", space_size);
                break;
            case IR_ASSIGN:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    move $t%d, $t%d\n", r0, r1);
                }
                break;
            case IR_ADD:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r2 = reg_load_operand(code->op2, f);
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    add $t%d, $t%d, $t%d\n", r0, r1, r2);
                }
                break;
            case IR_MINUS:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r2 = reg_load_operand(code->op2, f);
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    sub $t%d, $t%d, $t%d\n", r0, r1, r2);
                }
                break;
            case IR_MUL:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r2 = reg_load_operand(code->op2, f);
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    mul $t%d, $t%d, $t%d\n", r0, r1, r2);
                }
                break;
            case IR_DIV:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r2 = reg_load_operand(code->op2, f);
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    div $t%d, $t%d\n", r1, r2);
                    fprintf(f, "    mflo $t%d\n", r0);
                }
                break;
            case IR_ADDRESS:
                if (code->result.sym != NULL) {
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    addi $t%d, $fp, -%d\n", r0, space_size-lookup_offset(code->op1.sym->name));
                }
                break;
            case IR_POINTL:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r0 = reg_load_operand(code->result, f);
                    fprintf(f, "    sw $t%d, 0($t%d)\n", r1, r0);
                }
                break;
            case IR_POINTR:
                if (code->result.sym != NULL) {
                    r1 = reg_load_operand(code->op1, f);
                    r0 = link_symbol_register(code->result.sym, f);
                    fprintf(f, "    lw $t%d, 0($t%d)\n", r0, r1);
                }
                break;
            case IR_GOTO:
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    j %s\n", code->op1.sym->name);
                break;
            case IR_GOL:
                r1 = reg_load_operand(code->op1, f);
                r2 = reg_load_operand(code->op2, f);
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    blt $t%d, $t%d, %s\n", r1, r2, code->result.sym->name);
                break;
            case IR_GOG:
                r1 = reg_load_operand(code->op1, f);
                r2 = reg_load_operand(code->op2, f);
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    bgt $t%d, $t%d, %s\n", r1, r2, code->result.sym->name);
                break;
            case IR_GOLE:
                r1 = reg_load_operand(code->op1, f);
                r2 = reg_load_operand(code->op2, f);
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    ble $t%d, $t%d, %s\n", r1, r2, code->result.sym->name);
                break;
            case IR_GOGE:
                r1 = reg_load_operand(code->op1, f);
                r2 = reg_load_operand(code->op2, f);
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    bge $t%d, $t%d, %s\n", r1, r2, code->result.sym->name);
                break;
            case IR_GOE:
                r1 = reg_load_operand(code->op1, f);
                r2 = reg_load_operand(code->op2, f);
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    beq $t%d, $t%d, %s\n", r1, r2, code->result.sym->name);
                break;
            case IR_GONE:
                r1 = reg_load_operand(code->op1, f);
                r2 = reg_load_operand(code->op2, f);
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    bne $t%d, $t%d, %s\n", r1, r2, code->result.sym->name);
                break;
            case IR_RETURN:
                r1 = reg_load_operand(code->op1, f);
                fprintf(f, "    move $v0, $t%d\n", r1);
                fprintf(f, "    move $sp, $fp\n");
                fprintf(f, "    lw $fp, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                fprintf(f, "    jr $ra\n");
                break;
            case IR_ARG:
                r1 = reg_load_operand(code->op1, f);
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $t%d, 0($sp)\n", r1);
                break;
            case IR_CALL:
                for (i = 0; i < MAX_REGS; i++) {
                    dump_register(i, f);
                }
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $ra, 0($sp)\n");
                fprintf(f, "    jal %s\n", code->op1.sym->name);
                fprintf(f, "    lw $ra, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                r0 = link_symbol_register(code->result.sym, f);
                fprintf(f, "    move $t%d, $v0\n", r0);
                /* 这边没有跟踪参数空间，因此难以恢复。由于临时变量空间固定，因此从 fp 重新算！！！ */
                fprintf(f, "    addi $sp, $fp, -%d\n", space_size);
                break;
            case IR_PARAM:
                r1 = link_symbol_register(code->op1.sym, f);
                fprintf(f, "    lw $t%d, %d($fp)\n", r1, params_iter*4+8);
                params_iter++;
                break;
            case IR_READ:
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $ra, 0($sp)\n");
                fprintf(f, "    jal read\n");
                fprintf(f, "    lw $ra, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                r1 = link_symbol_register(code->op1.sym, f);
                fprintf(f, "    move $t%d, $v0\n", r1);
                break;
            case IR_WRITE:
                r1 = reg_load_operand(code->op1, f);
                fprintf(f, "    move $a0, $t%d\n", r1);
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $ra, 0($sp)\n");
                fprintf(f, "    jal write\n");
                fprintf(f, "    lw $ra, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                break;
            default:
                break;
        }
        // printf("Code %d: %d\n", ++count, code->kind);
        // print_regs();
    }
}

bool generate_components(FILE *f)
{
    fprintf(f, ".data\n");
    fprintf(f, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(f, "_ret: .asciiz \"\\n\"\n");
    fprintf(f, ".globl main\n");
    fprintf(f, ".text\n");
    if (copy_from_file("read.s", f) == false) return false;
    if (copy_from_file("write.s", f) == false) return false;
    return true;
}

int calculate_offset()
{
    int offset = 0;
    struct Symbol_t *sym = sym_table;
    for (; sym != NULL; sym = sym->next) {
        if (sym->kind == SYM_VAR) {
            sym->offset = offset;
            offset += 4;
        }
        else if (sym->kind == SYM_ARRAY) {
            sym->offset = offset;
            offset += sym->array_width;
        }
    }
    return offset;
}

int lookup_offset(char *name)
{
    struct Symbol_t *sym = sym_table;
    for (; sym != NULL; sym = sym->next) {
        if (strcmp(sym->name, name) == 0) {
            return sym->offset;
        }
    }
    return -1;
}

bool copy_from_file(char source_file[], FILE *f)
{
    FILE *assemble_source = fopen(source_file, "r");
    if (!assemble_source) {
        perror("Missing files!");
        return false;
    }
    char buffer[FILE_LINE_BUFFER];
    while (fgets(buffer, sizeof buffer, assemble_source)) {
        fprintf(f, "%s", buffer);
    }
    fprintf(f, "\n");
    fclose(assemble_source);
    return true;
}

void initialize_code_generation()
{
    int i;
    for (i = 0; i < MAX_REGS; i++) {
        regs[i].free = true;
    }
    space_size = calculate_offset();
    reg_scan = 0;
}

int reg_load_operand(struct Operand op, FILE *f)
{
    if (op.kind == OP_CONSTANT) {
        int reg = reg_scan++;
        if (reg_scan >= GEN_REGS) reg_scan = 0;
        dump_register(reg, f);
        fprintf(f, "    li $t%d, %d\n", reg, op.value);
        regs[reg].free = false;
        regs[reg].value = true;
        return reg;
    }
    else {
        struct Symbol_t *sym = op.sym;
        int i = 0;
        for (; i < GEN_REGS; i++) {
            if (regs[i].free == false && regs[i].sym == sym) break;
        }
        if (i == GEN_REGS) {
            // 不在寄存器中，要把它加入寄存器
            int reg = reg_scan++;
            if (reg_scan >= GEN_REGS) reg_scan = 0;
            dump_register(reg, f);
            load_register_symbol(sym, reg, f);
            return reg;
        }
        else {
            // 该符号已经在寄存器中，直接返回相应寄存器
            return i;
        }
    }
}

int link_symbol_register(struct Symbol_t *sym, FILE *f)
{
    int i = 0;
    for (; i < GEN_REGS; i++) {
        if (regs[i].free == false && regs[i].sym == sym) break;
    }
    if (i == GEN_REGS) {
        int reg = reg_scan++;
        if (reg_scan >= GEN_REGS) reg_scan = 0;
        dump_register(reg, f);
        regs[reg].free = false;
        regs[reg].value = false;
        regs[reg].sym = sym;
        sym->dirty = true;
        return reg;
    }
    else {
        sym->dirty = true;
        return i;
    }
}

void dump_register(int reg, FILE* f)
{
    if (regs[reg].free == true) return;
    if (regs[reg].value == true) {
        regs[reg].free = true;
        return;
    }
    struct Symbol_t *sym = regs[reg].sym;
    if (sym->dirty == true) {
        store_register_symbol(sym, reg, f);
    }
    regs[reg].free = true;
    return;
}

void load_register_symbol(struct Symbol_t *sym, int reg, FILE *f)
{
    int offset = lookup_offset(sym->name);
    fprintf(f, "    lw $t%d, -%d($fp)\n", reg, space_size-offset);
    sym->dirty = false;
    regs[reg].free = false;
    regs[reg].value = false;
    regs[reg].sym = sym;
}

void store_register_symbol(struct Symbol_t *sym, int reg, FILE *f)
{
    int offset = lookup_offset(sym->name);
    fprintf(f, "    sw $t%d, -%d($fp)\n", reg, space_size-offset);
    sym->dirty = false;
}

void generate_code(CB ic, FILE *f)
{
    initialize_code_generation();
    generate_components(f);
    generate_text(ic, f);
}

void print_regs()
{
    int i;
    for (i = 0; i < GEN_REGS; i++) {
        if (regs[i].free == false && regs[i].value == false) {
            printf("%d: %s\n", i, regs[i].sym->name);
        }
    }
    printf("\n");
}