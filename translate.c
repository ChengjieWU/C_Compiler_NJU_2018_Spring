#include <stdio.h>
#include <string.h>
#include <mm_malloc.h>
#include "translate.h"

bool display = true;

void write_CB(CB cb, FILE *f);
CB append_CB(CB a, CB b);
CB translate_IR();
CB translate_function(struct Node *extdef);
CB translate_FunDec(struct Node *fundec);
CB translate_CompSt(struct Node *compst);

struct InterCode *new_code(enum ir_kind kind);
struct Symbol_t *sym_table;
struct Symbol_t *add_sym_table(char *name, enum sym_kind kind);
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

void initialize_sym_table()
{
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

void write_CB(CB cb, FILE *f)
{
    struct InterCode *p = cb.begin;
    for (; p != NULL; p = p->next) {
        char buffer[MAX_CODE_LEN];
        switch (p->kind) {
            case IR_FUNCTION:
                sprintf(buffer, "FUNCTION %s :", p->op1.sym->name);
                break;
            case IR_PARAM:
                sprintf(buffer, "PARAM %s", p->op1.sym->name);
                break;
        }
        fprintf(f, "%s\n", buffer);
        if (display) printf("%s\n", buffer);
    }
    return;
}
