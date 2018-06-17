#include <stdio.h>
#include <string.h>
#include <mm_malloc.h>
#include "translate.h"

/* 需要注意的是，此种实现下， */

bool display = true;

struct Symbol_t *arg_list;

void write_CB(CB cb, FILE *f);
void format_operand_output(char *buffer, struct Operand op);
CB append_CB(CB a, CB b);
CB translate_IR();
CB translate_function(struct Node *extdef);
CB translate_FunDec(struct Node *fundec);
CB translate_CompSt(struct Node *compst);
CB translate_DefList(struct Node *deflist);
CB translate_StmtList(struct Node *stmtlist);
CB translate_Stmt(struct Node *stmt);
CB translate_Exp(struct Node *exp, struct Symbol_t *place);
CB translate_Cond(struct Node *exp, struct Symbol_t *label_true, struct Symbol_t *label_false);
CB translate_Args(struct Node *args);

CB generate_label_code(struct Symbol_t *label);
CB generate_goto_code(struct Symbol_t *label);
struct InterCode *new_code(enum ir_kind kind);
struct Symbol_t *sym_table;
int temp_variable_count;
int label_count;
struct Symbol_t *add_sym_table(char *name, enum sym_kind kind);
struct Symbol_t *new_temp();
struct Symbol_t *new_label();
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
            write_CB(cb, NULL);
            printf("-----------------------------------------------");
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
    for (; stmtlist->num_children != 0; stmtlist = stmtlist->children[1]) {
        struct Node *stmt = stmtlist->children[0];
        cb = append_CB(cb, translate_Stmt(stmt));
    }
    return cb;
}

CB translate_Stmt(struct Node *stmt)
{
    CB cb;
    cb.begin = cb.end = NULL;
    if (strcmp(stmt->children[0]->type, "Exp") == 0) {
        cb = append_CB(cb, translate_Exp(stmt->children[0], NULL));
    }
    else if (strcmp(stmt->children[0]->type, "CompSt") == 0) {
        cb = append_CB(cb, translate_CompSt(stmt->children[0]));
    }
    else if (strcmp(stmt->children[0]->type, "RETURN") == 0) {
        struct Symbol_t *t1 = new_temp();
        cb = append_CB(cb, translate_Exp(stmt->children[1], t1));
        struct InterCode *code = new_code(IR_RETURN);
        code->op1.kind = OP_VAR;
        code->op1.sym = t1;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
    }
    else if (strcmp(stmt->children[0]->type, "WHILE") == 0) {
        struct Symbol_t *label1 = new_label();
        struct Symbol_t *label2 = new_label();
        struct Symbol_t *label3 = new_label();
        cb = append_CB(cb, generate_label_code(label1));
        cb = append_CB(cb, translate_Cond(stmt->children[2], label2, label3));
        cb = append_CB(cb, generate_label_code(label2));
        cb = append_CB(cb, translate_Stmt(stmt->children[4]));
        cb = append_CB(cb, generate_goto_code(label1));
        cb = append_CB(cb, generate_label_code(label3));
    }
    else if (stmt->num_children == 5 && strcmp(stmt->children[0]->type, "IF") == 0) {
        struct Symbol_t *label1 = new_label();
        struct Symbol_t *label2 = new_label();
        cb = append_CB(cb, translate_Cond(stmt->children[2], label1, label2));
        cb = append_CB(cb, generate_label_code(label1));
        cb = append_CB(cb, translate_Stmt(stmt->children[4]));
        cb = append_CB(cb, generate_label_code(label2));
    }
    else if (stmt->num_children == 7 && strcmp(stmt->children[0]->type, "IF") == 0) {
        struct Symbol_t *label1 = new_label();
        struct Symbol_t *label2 = new_label();
        struct Symbol_t *label3 = new_label();
        cb = append_CB(cb, translate_Cond(stmt->children[2], label1, label2));
        cb = append_CB(cb, generate_label_code(label1));
        cb = append_CB(cb, translate_Stmt(stmt->children[4]));
        cb = append_CB(cb, generate_goto_code(label3));
        cb = append_CB(cb, generate_label_code(label2));
        cb = append_CB(cb, translate_Stmt(stmt->children[6]));
        cb = append_CB(cb, generate_label_code(label3));
    }
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
            cb.begin = cb.end = code;
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
    else if (strcmp(exp->children[1]->type, "RELOP") == 0 || 
            strcmp(exp->children[1]->type, "AND") == 0 || 
            strcmp(exp->children[1]->type, "OR") == 0 || 
            strcmp(exp->children[0]->type, "NOT") == 0) {
        struct Symbol_t *label1 = new_label();
        struct Symbol_t *label2 = new_label();
        struct InterCode *code = new_code(IR_ASSIGN);
        code->op1.kind = OP_CONSTANT;
        code->op1.value = 0;
        code->result.kind = OP_VAR;
        code->result.sym = place;
        cb.begin = cb.end = code;

        cb = append_CB(cb, translate_Cond(exp, label1, label2));

        code = new_code(IR_LABEL);
        code->op1.kind = OP_LABEL;
        code->op1.sym = label1;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);

        code = new_code(IR_ASSIGN);
        code->op1.kind = OP_CONSTANT;
        code->op1.value = 1;
        code->result.kind = OP_VAR;
        code->result.sym = place;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);

        code = new_code(IR_LABEL);
        code->op1.kind = OP_LABEL;
        code->op1.sym = label2;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
    }
    else if (strcmp(exp->children[2]->type, "RP") == 0) {
        struct Symbol_t *function = search_sym_table(exp->children[0]->type_string);
        if (strcmp(function->name, "read") == 0) {
            struct InterCode *code = new_code(IR_READ);
            code->op1.kind = OP_VAR;
            code->op1.sym = place;
            cb.begin = cb.end = code;
        }
        else {
            struct InterCode *code = new_code(IR_CALL);
            code->op1.kind = OP_FUNC;
            code->op1.sym = function;
            code->result.kind = OP_VAR;
            code->result.sym = place;
            cb.begin = cb.end = code;
        }
    }
    else if (strcmp(exp->children[2]->type, "Args") == 0) {
        struct Symbol_t *function = search_sym_table(exp->children[0]->type_string);
        arg_list = NULL;
        cb = translate_Args(exp->children[2]);
        if (strcmp(function->name, "write") == 0) {
            struct InterCode *code = new_code(IR_WRITE);
            code->op1.kind = OP_VAR;
            if (arg_list != NULL) code->op1.sym = arg_list;
            else perror("wrong write usage");
            CB cbt;
            cbt.begin = cbt.end = code;
            cb = append_CB(cb, cbt);
        }
        else {
            for (; arg_list != NULL; arg_list = arg_list->arg_next) {
                struct InterCode *code = new_code(IR_ARG);
                code->op1.kind = OP_VAR;
                code->op1.sym = arg_list;
                CB cbt;
                cbt.begin = cbt.end = code;
                cb = append_CB(cb, cbt);
            }
            struct InterCode *code = new_code(IR_CALL);
            code->op1.kind = OP_FUNC;
            code->op1.sym = function;
            code->result.kind = OP_VAR;
            code->result.sym = place;
            CB cbt;
            cbt.begin = cbt.end = code;
            cb = append_CB(cb, cbt);
        }
    }
    return cb;
}

CB translate_Cond(struct Node *exp, struct Symbol_t *label_true, struct Symbol_t *label_false)
{
    CB cb;
    cb.begin = cb.end = NULL;
    if (strcmp(exp->children[0]->type, "NOT") == 0) {
        cb = translate_Cond(exp, label_false, label_true);
    }
    else if (exp->num_children > 1 && strcmp(exp->children[1]->type, "AND") == 0) {
        struct Symbol_t *label1 = new_label();
        cb = translate_Cond(exp->children[0], label1, label_false);
        struct InterCode *code = new_code(IR_LABEL);
        code->op1.kind = OP_LABEL;
        code->op1.sym = label1;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
        cb = append_CB(cb, translate_Cond(exp->children[2], label_true, label_false));
    }
    else if (exp->num_children > 1 && strcmp(exp->children[1]->type, "OR") == 0) {
        struct Symbol_t *label1 = new_label();
        cb = translate_Cond(exp->children[0], label_true, label1);
        struct InterCode *code = new_code(IR_LABEL);
        code->op1.kind = OP_LABEL;
        code->op1.sym = label1;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
        cb = append_CB(cb, translate_Cond(exp->children[2], label_true, label_false));
    }
    else if (exp->num_children > 1 && strcmp(exp->children[1]->type, "RELOP") == 0) {
        struct Symbol_t *t1 = new_temp();
        struct Symbol_t *t2 = new_temp();
        cb = translate_Exp(exp->children[0], t1);
        cb = append_CB(cb, translate_Exp(exp->children[2], t2));
        struct InterCode *code;
        if (strcmp(exp->children[1]->type_string, "<") == 0) code = new_code(IR_GOL);
        else if (strcmp(exp->children[1]->type_string, ">") == 0) code = new_code(IR_GOG);
        else if (strcmp(exp->children[1]->type_string, "<=") == 0) code = new_code(IR_GOLE);
        else if (strcmp(exp->children[1]->type_string, ">=") == 0) code = new_code(IR_GOGE);
        else if (strcmp(exp->children[1]->type_string, "==") == 0) code = new_code(IR_GOE);
        else if (strcmp(exp->children[1]->type_string, "!=") == 0) code = new_code(IR_GONE);
        else printf("relop error!\n");
        code->op1.kind = OP_VAR;
        code->op1.sym = t1;
        code->op2.kind = OP_VAR;
        code->op2.sym = t2;
        code->result.kind = OP_LABEL;
        code->result.sym = label_true;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
        cb = append_CB(cb, generate_goto_code(label_false));
    }
    else {
        struct Symbol_t *t1 = new_temp();
        cb = translate_Exp(exp, t1);
        struct InterCode *code = new_code(IR_GONE);
        code->op1.kind = OP_VAR;
        code->op1.sym = t1;
        code->op2.kind = OP_CONSTANT;
        code->op2.value = 0;
        code->result.kind = OP_LABEL;
        code->result.sym = label_true;
        CB cbt;
        cbt.begin = cbt.end = code;
        cb = append_CB(cb, cbt);
        cb = append_CB(cb, generate_goto_code(label_false));
    }
    return cb;
}

CB translate_Args(struct Node *args)
{
    CB cb;
    cb.begin = cb.end = NULL;
    if (args->num_children == 1) {
        struct Symbol_t *t1 = new_temp();
        cb = translate_Exp(args->children[0], t1);
        t1->arg_next = arg_list;
        arg_list = t1;
    }
    else {
        struct Symbol_t *t1 = new_temp();
        cb = translate_Exp(args->children[0], t1);
        if (arg_list == NULL) {
            t1->arg_next = NULL;
            arg_list = t1;
        }
        else {
            t1->arg_next = arg_list;
            arg_list = t1;
        }
        cb = append_CB(cb, translate_Args(args->children[2]));
    }
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

struct Symbol_t *new_label()
{
    char name_buffer[SYMBOL_T_NAME_MAX_LEN];
    sprintf(name_buffer, "__LABEL_%d__", label_count++);
    return add_sym_table(name_buffer, SYM_LABEL);
}

void initialize_sym_table()
{
    label_count = 0;
    temp_variable_count = 0;
    sym_table = NULL;   // Cause problems to memory when analyzing the second one.
    add_sym_table("read", SYM_FUNC);
    add_sym_table("write", SYM_FUNC);
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

CB generate_label_code(struct Symbol_t *label)
{
    struct InterCode *code = new_code(IR_LABEL);
    code->op1.kind = OP_LABEL;
    code->op1.sym = label;
    CB cbt;
    cbt.begin = cbt.end = code;
    return cbt;
}

CB generate_goto_code(struct Symbol_t *label)
{
    struct InterCode *code = new_code(IR_GOTO);
    code->op1.kind = OP_LABEL;
    code->op1.sym = label;
    CB cbt;
    cbt.begin = cbt.end = code;
    return cbt;
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
            case IR_LABEL:
                sprintf(buffer, "LABEL %s :", p->op1.sym->name);
                break;
            case IR_FUNCTION:
                sprintf(buffer, "FUNCTION %s :", p->op1.sym->name);
                break;
            case IR_ASSIGN:
                format_operand_output(op1_buffer, p->op1);
                if (p->result.sym != NULL)
                    sprintf(buffer, "%s := %s", p->result.sym->name, op1_buffer);
                break;
            case IR_ADD:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                if (p->result.sym != NULL)
                    sprintf(buffer, "%s := %s + %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_MINUS:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                if (p->result.sym != NULL)
                    sprintf(buffer, "%s := %s - %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_MUL:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                if (p->result.sym != NULL)
                    sprintf(buffer, "%s := %s * %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_DIV:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                if (p->result.sym != NULL)
                    sprintf(buffer, "%s := %s / %s", p->result.sym->name, op1_buffer, op2_buffer);
                break;
            case IR_GOTO:
                sprintf(buffer, "GOTO %s", p->op1.sym->name);
                break;
            case IR_GOL:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "IF %s < %s GOTO %s", op1_buffer, op2_buffer, p->result.sym->name);
                break;
            case IR_GOG:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "IF %s > %s GOTO %s", op1_buffer, op2_buffer, p->result.sym->name);
                break;
            case IR_GOLE:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "IF %s <= %s GOTO %s", op1_buffer, op2_buffer, p->result.sym->name);
                break;
            case IR_GOGE:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "IF %s >= %s GOTO %s", op1_buffer, op2_buffer, p->result.sym->name);
                break;
            case IR_GOE:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "IF %s == %s GOTO %s", op1_buffer, op2_buffer, p->result.sym->name);
                break;
            case IR_GONE:
                format_operand_output(op1_buffer, p->op1);
                format_operand_output(op2_buffer, p->op2);
                sprintf(buffer, "IF %s != %s GOTO %s", op1_buffer, op2_buffer, p->result.sym->name);
                break;
            case IR_RETURN:
                format_operand_output(op1_buffer, p->op1);
                sprintf(buffer, "RETURN %s", op1_buffer);
                break;
            case IR_ARG:
                format_operand_output(op1_buffer, p->op1);
                sprintf(buffer, "ARG %s", op1_buffer);
                break;
            case IR_CALL:
                sprintf(buffer, "%s := CALL %s", p->result.sym->name, p->op1.sym->name);
                break;
            case IR_PARAM:
                sprintf(buffer, "PARAM %s", p->op1.sym->name);
                break;
            case IR_READ:
                format_operand_output(op1_buffer, p->op1);
                sprintf(buffer, "READ %s", op1_buffer);
                break;
            case IR_WRITE:
                format_operand_output(op1_buffer, p->op1);
                sprintf(buffer, "WRITE %s", op1_buffer);
                break;
            default:
                break;
        }
        if (f != NULL) fprintf(f, "%s\n", buffer);
        if (display) printf("%s\n", buffer);
    }
    return;
}
