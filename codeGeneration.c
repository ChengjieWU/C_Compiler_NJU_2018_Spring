#include "codeGeneration.h"

extern struct Symbol_t *sym_table;
int params_count = 0;
int space_size = 0;

void generate_text(CB ic, FILE *f)
{
    struct InterCode *code = ic.begin;
    for (; code != NULL; code = code->next) {
        switch (code->kind) {
            case IR_LABEL:
                fprintf(f, "%s:\n", code->op1.sym->name);
                break;
            case IR_FUNCTION:
                fprintf(f, "%s:\n", code->op1.sym->name);
                params_count = code->op1.sym->num_params;
                break;
            case IR_ASSIGN:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    store_register_symbol(code->result.sym, 0, f);
                }
                break;
            case IR_ADD:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    load_operand(code->op2, 1, f);
                    fprintf(f, "    add $t2, $t0, $t1\n");
                    store_register_symbol(code->result.sym, 2, f);
                }
                break;
            case IR_MINUS:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    load_operand(code->op2, 1, f);
                    fprintf(f, "    sub $t2, $t0, $t1\n");
                    store_register_symbol(code->result.sym, 2, f);
                }
                break;
            case IR_MUL:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    load_operand(code->op2, 1, f);
                    fprintf(f, "    mul $t2, $t0, $t1\n");
                    store_register_symbol(code->result.sym, 2, f);
                }
                break;
            case IR_DIV:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    load_operand(code->op2, 1, f);
                    fprintf(f, "    div $t0, $t1\n");
                    fprintf(f, "    mflo $t2\n");
                    store_register_symbol(code->result.sym, 2, f);
                }
                break;
            case IR_ADDRESS:
                if (code->result.sym != NULL) {
                    fprintf(f, "    la $t0, ASSPRE_%s\n", code->op1.sym->name);
                    store_register_symbol(code->result.sym, 0, f);
                }
                break;
            case IR_POINTL:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    load_operand(code->result, 1, f);
                    fprintf(f, "    sw $t0, 0($t1)\n");
                }
                break;
            case IR_POINTR:
                if (code->result.sym != NULL) {
                    load_operand(code->op1, 0, f);
                    fprintf(f, "    lw $t1, 0($t0)\n");
                    store_register_symbol(code->result.sym, 1, f);
                }
                break;
            case IR_GOTO:
                fprintf(f, "    j %s\n", code->op1.sym->name);
                break;
            case IR_GOL:
                load_operand(code->op1, 0, f);
                load_operand(code->op2, 1, f);
                fprintf(f, "    blt $t0, $t1, %s\n", code->result.sym->name);
                break;
            case IR_GOG:
                load_operand(code->op1, 0, f);
                load_operand(code->op2, 1, f);
                fprintf(f, "    bgt $t0, $t1, %s\n", code->result.sym->name);
                break;
            case IR_GOLE:
                load_operand(code->op1, 0, f);
                load_operand(code->op2, 1, f);
                fprintf(f, "    ble $t0, $t1, %s\n", code->result.sym->name);
                break;
            case IR_GOGE:
                load_operand(code->op1, 0, f);
                load_operand(code->op2, 1, f);
                fprintf(f, "    bge $t0, $t1, %s\n", code->result.sym->name);
                break;
            case IR_GOE:
                load_operand(code->op1, 0, f);
                load_operand(code->op2, 1, f);
                fprintf(f, "    beq $t0, $t1, %s\n", code->result.sym->name);
                break;
            case IR_GONE:
                load_operand(code->op1, 0, f);
                load_operand(code->op2, 1, f);
                fprintf(f, "    bne $t0, $t1, %s\n", code->result.sym->name);
                break;
            case IR_RETURN:
                load_operand(code->op1, 0, f);
                fprintf(f, "    move $v0, $t0\n");
                fprintf(f, "    jr $ra\n");
                break;
            case IR_ARG:
                load_operand(code->op1, 0, f);
                if (params_count < 4) {
                    fprintf(f, "    move $a%d, $t0\n", params_count++);
                }
                else {
                    fprintf(f, "    addi $sp, $sp, -4\n");
                    fprintf(f, "sw $t0, 0($sp)\n");
                    params_count++;
                }
                break;
            case IR_CALL:
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $ra, 0($sp)\n");
                fprintf(f, "    jal %s\n", code->op1.sym->name);
                fprintf(f, "    lw $ra, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                fprintf(f, "    move $t0, $v0\n");
                store_register_symbol(code->result.sym, 0, f);
                break;
            case IR_PARAM:
                if (params_count <= 4) {
                    fprintf(f, "    move $t0, $a%d\n", --params_count);
                }
                else {
                    fprintf(f, "    lw $t0, %d($sp)\n", (params_count-4)*4);
                    params_count--;
                }
                store_register_symbol(code->op1.sym, 0, f);
                break;
            case IR_READ:
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $ra, 0($sp)\n");
                fprintf(f, "    jal read\n");
                fprintf(f, "    lw $ra, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                fprintf(f, "    move $t0, $v0\n");
                store_register_symbol(code->op1.sym, 0, f);
                break;
            case IR_WRITE:
                load_operand(code->op1, 0, f);
                fprintf(f, "    move $a0, $t0\n");
                fprintf(f, "    addi $sp, $sp, -4\n");
                fprintf(f, "    sw $ra, 0($sp)\n");
                fprintf(f, "    jal write\n");
                fprintf(f, "    lw $ra, 0($sp)\n");
                fprintf(f, "    addi $sp, $sp, 4\n");
                break;
            default:
                break;
        }
    }
}

bool generate_components(FILE *f)
{
    fprintf(f, ".data\n");
    fprintf(f, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(f, "_ret: .asciiz \"\\n\"\n");
    struct Symbol_t *sym = sym_table;
    for (; sym != NULL; sym = sym->next) {
        if (sym->kind == SYM_VAR) {
            fprintf(f, "ASSPRE_%s: .word 0\n", sym->name);
        }
        else if (sym->kind == SYM_ARRAY) {
            fprintf(f, "ASSPRE_%s: .space %d\n", sym->name, sym->array_width);
        }
    }
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
    params_count = 0;
    space_size = calculate_offset();
}

void load_operand(struct Operand op, int reg, FILE *f) {
    if (op.kind == OP_CONSTANT) {
        fprintf(f, "    li $t%d, %d\n", reg, op.value);
    }
    else {
        load_register_symbol(op.sym, reg, f);
    }
}

void load_register_symbol(struct Symbol_t *sym, int reg, FILE *f)
{
    regs[reg].free = false;
    regs[reg].sym = sym;
    fprintf(f, "    la $s0, ASSPRE_%s\n", sym->name);
    fprintf(f, "    lw $t%d, 0($s0)\n", reg);
}

void store_register_symbol(struct Symbol_t *sym, int reg, FILE *f)
{
    fprintf(f, "    la $s0, ASSPRE_%s\n", sym->name);
    fprintf(f, "    sw $t%d, 0($s0)\n", reg);
}

void generate_code(CB ic, FILE *f)
{
    initialize_code_generation();
    generate_components(f);
    generate_text(ic, f);
}