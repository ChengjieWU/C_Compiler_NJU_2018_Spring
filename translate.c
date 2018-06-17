#include <stdio.h>
#include <string.h>
#include <mm_malloc.h>
#include "translate.h"

bool display = true;

void write_CB(CB cb, FILE *f);
void format_operand_output(char *buffer, struct Operand op);
CB append_CB(CB a, CB b);
CB translate_IR();
CB translate_function(struct Node *extdef);
CB translate_FunDec(struct Node *fundec);
CB translate_CompSt(struct Node *compst);
CB translate_DefList(struct Node *deflist);
CB translate_StmtList(struct Node *stmtlist);
CB translate_Exp(struct Node *exp, struct Symbol_t *place);

struct InterCode *new_code(enum ir_kind kind);
struct Symbol_t *sym_table;
int temp_variable_count;
struct Symbol_t *add_sym_table(char *name, enum sym_kind kind);
struct Symbol_t *new_temp();
struct Symbol_t *search_sym_table(char *name);
void initialize_sym_table();

CB translate_IR()
{
    CB cb;
    cb.begin = cb.end = NULL;
    initialize_sym_table();
    struct Node *extdeflist = grammarTreeRoot->children[0];
    while (extdeflist->num_children != 0) {
        struct Node *extdef = extdeflist->children[0];
        if (strcmp(extdef->children[1]->type, "ExtDecList\0") == 0) {
            printf("Global variable is not implemented.\n");
        }
        else if (strcmp(extdef->children[1]->type, "SEMI\0") == 0) {
            printf("Structure is not implemented.\n");
        }
        else if (strcmp(extdef->children[1]->type, "FunDec\0") == 0)  {
            cb = append_CB(cb, translate_function(extdef));
        }
        extdeflist = extdeflist->children[1];
    }
    return cb;
}

CB translate_function(struct Node *extdef)
{
    CB cb;
    cb.begin = cb.end = NULL;
    struct Node *fundec = extdef->children[1];
    struct Node *compst = extdef->children[2];
    cb = append_CB(cb, translate_FunDec(fundec));
    cb = append_CB(cb, translate_CompSt(compst));
    return cb;
}

CB translate_FunDec(struct Node *fundec)
{
    CB cb;
    cb.begin = cb.end = NULL;
    struct InterCode *p = new_code(IR_FUNCTION);
    p->op1.kind = OP_FUNC;
    p->op1.sym = add_sym_table(fundec->children[0]->type_string, SYM_FUNC);
    cb.begin = p;
    cb.end = p;
    if (fundec->num_children == 4) {
        struct Node *varlist = fundec->children[2];
        while (varlist != NULL) {
            struct Node *id = varlist->children[0]->children[1]->children[0];
            p = new_code(IR_PARAM);
            p->op1.kind = OP_VAR;
            p->op1.sym = add_sym_table(id->type_string, SYM_VAR);
            cb.end->next = p;
            cb.end = p;
            if (varlist->num_children == 3) varlist = varlist->children[2];
            else break;
        }
    }
    return cb;
}

CB translate_CompSt(struct Node *compst)
{
    CB cb;
    cb.begin = cb.end = NULL;
    cb = append_CB(cb, translate_DefList(compst->children[1]));
    cb = append_CB(cb, translate_StmtList(compst->children[2]));
    return cb;
}

CB translate_DefList(struct Node *deflist)
{
    CB cb;
    cb.begin = cb.end = NULL;
    for (; deflist->num_children != 0; deflist = deflist->children[1]) {
        struct Node *declist = deflist->children[0]->children[1];
        while (declist != NULL) {
            struct Node *dec = declist->children[0];
            struct Node *id = dec->children[0]->children[0];
            struct Symbol_t *sym = add_sym_table(id->type_string, SYM_VAR);
            if (dec->num_children == 3) {
                struct Node *exp = dec->children[2];
                cb = append_CB(cb, translate_Exp(exp, sym));
            }
            if (declist->num_children == 3) declist = declist->children[2];
            else break;
        }
    }
    return cb;
}

CB translate_StmtList(struct Node *stmtlist)
{
    CB cb;
    cb.begin = cb.end = NULL;
    /**/
    return cb;
}

CB translate_Exp(struct Node *exp, struct Symbol_t *place)
{
    CB cb;
    cb.begin = cb.end = NULL;
    if (exp->num_children == 1) {
        if (strcmp(exp->children[0]->type, "INT\0") == 0) {
            int value = exp->children[0]->type_int;
            struct InterCode *code = new_code(IR_ASSIGN);
            code->op1.kind = OP_CONSTANT;
            code->op1.value = value;
            code->result.kind = OP_VAR;
            code->result.sym = place;
            cb.begin = code;
            cb.end = code;
        }
        else if (strcmp(exp->children[0]->type, "ID\0") == 0) {
            struct Symbol_t *sym = search_sym_table(exp->children[0]->type_string);
            struct InterCode *code = new_code(IR_ASSIGN);
            code->op1.kind = OP_VAR;
            code->op1.sym = sym;
            code->result.kind = OP_VAR;
            code->result.sym = place;
            cb.begin = code;
            cb.end = code;
        }
    }
    else if (strcmp(exp->children[1]->type, "ASSIGNOP\0") == 0) {
        struct Symbol_t *variable = search_sym_table(exp->children[0]->children[0]->type_string);
        struct Symbol_t *t1 = new_temp();
        cb = translate_Exp(exp->children[2], t1);
        CB cbt;
        struct InterCode *code = new_code(IR_ASSIGN);
        code->op1.kind = OP_VAR;
        code->op1.sym = t1;
        code->result.kind = OP_VAR;
        code->result.sym = variable;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
        code = new_code(IR_ASSIGN);
        code->op1.kind = OP_VAR;
        code->op1.sym = variable;
        code->result.kind = OP_VAR;
        code->result.sym = place;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
    }
    else if (strcmp(exp->children[1]->type, "PLUS") == 0 || 
            strcmp(exp->children[1]->type, "MINUS") == 0 || 
            strcmp(exp->children[1]->type, "STAR") == 0 || 
            strcmp(exp->children[1]->type, "DIV") == 0) {
        struct Symbol_t *t1 = new_temp();
        struct Symbol_t *t2 = new_temp();
        cb = append_CB(cb, translate_Exp(exp->children[0], t1));
        cb = append_CB(cb, translate_Exp(exp->children[2], t2));
        struct InterCode *code;
        if (strcmp(exp->children[1]->type, "PLUS") == 0) code = new_code(IR_ADD);
        else if (strcmp(exp->children[1]->type, "MINUS") == 0) code = new_code(IR_MINUS);
        else if (strcmp(exp->children[1]->type, "STAR") == 0) code = new_code(IR_MUL);
        else if (strcmp(exp->children[1]->type, "DIV") == 0) code = new_code(IR_DIV);
        code->op1.kind = OP_VAR;
        code->op1.sym = t1;
        code->op2.kind = OP_VAR;
        code->op2.sym = t2;
        code->result.kind = OP_VAR;
        code->result.sym = place;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
    }
    else if (strcmp(exp->children[0]->type, "MINUS") == 0) {
        struct Symbol_t *t1 = new_temp();
        cb = translate_Exp(exp->children[1], t1);
        struct InterCode *code = new_code(IR_MINUS);
        code->op1.kind = OP_CONSTANT;
        code->op1.value = 0;
        code->op2.kind = OP_VAR;
        code->op2.sym = t1;
        code->result.kind = OP_VAR;
        code->result.sym = place;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
    }
    else if (strcmp(exp->children[0]->type, "LP") == 0) {
        cb = translate_Exp(exp->children[1], place);
    }
    /**/
    return cb;
}



struct InterCode *new_code(enum ir_kind kind)
{
    struct InterCode *p = (struct InterCode *)malloc(sizeof(struct InterCode));
    p->kind = kind;
    p->next = NULL;
    return p;
}

struct Symbol_t *add_sym_table(char *name, enum sym_kind kind)
{
    struct Symbol_t *p = (struct Symbol_t *)malloc(sizeof(struct Symbol_t));
    strcpy(p->name, name);
    p->kind = kind;
    p->next = sym_table;
    sym_table = p;
    return p;
}

struct Symbol_t *search_sym_table(char *name)
{
    struct Symbol_t *p = sym_table;
    for (; p != NULL; p = p->next) {
        if (strcmp(name, p->name) == 0) break;
    }
    return p;
}

struct Symbol_t *new_temp()
{
    char name_buffer[SYMBOL_T_NAME_MAX_LEN];
    sprintf(name_buffer, "__temp_variable_%d__", temp_variable_count++);
    return add_sym_table(name_buffer, SYM_VAR);
}

void initialize_sym_table()
{
    temp_variable_count = 0;
    sym_table = NULL;   // Cause problems to memory when analyzing the second one.
    add_sym_table("READ", SYM_FUNC);
    add_sym_table("WRITE", SYM_FUNC);
}

CB append_CB(CB a, CB b)
{
    if (a.begin == NULL) {
        return b;
    }
    else if (b.begin == NULL){
        return a;
    }
    else {
        a.end->next = b.begin;
        a.end = b.end;
        return a;
    }
}

void format_operand_output(char *buffer, struct Operand op) {
    if (op.kind == OP_CONSTANT) {
        sprintf(buffer, "#%d\0", op.value);
    }
    else if (op.kind == OP_VAR) {
        sprintf(buffer, "%s\0", op.sym->name);
    }
    else {
        printf("Not implemented. In format_operand_output.\n");
    }
}

void write_CB(CB cb, FILE *f)
{
    struct InterCode *p = cb.begin;
    for (; p != NULL; p = p->next) {
        char buffer[MAX_CODE_LEN];
        char op1_buffer[MAX_CODE_LEN];
        char op2_buffer[MAX_CODE_LEN];
        switch (p->kind) {
            case IR_FUNCTION:
                sprintf(buffer, "FUNCTION %s :", p->op1.sym->name);
                break;
            case IR_ASSIGN:
                format_operand_output(op1_buffer, p->op1);
                sprintf(buffer, "%s := %s", p->result.sym->name, op1_buffer);
                break;
            case IR_ADD:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "%s := %s + %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_MINUS:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "%s := %s - %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_MUL:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "%s := %s * %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_DIV:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "%s := %s / %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_PARAM:
                sprintf(buffer, "PARAM %s", p->op1.sym->name);
                break;
            default:
                break;
        }
        // fprintf(f, "%s\n", buffer);
        if (display) printf("%s\n", buffer);
    }
    return;
}
