#include <string.h>
#include <stdio.h>
#include <mm_malloc.h>
#include "grammarTree.h"
#include "type.h"
#include "symbol.h"
#include "semantic.h"

void semantic_analysis();
void semantic_global_variable(struct Node *extdef);
void semantic_structure_definition(struct Node *extdef);
void semantic_function_definition(struct Node* extdef);
void semantic_ExtDecList(struct Node *extdeclist, struct Type *type);
struct Symbol *semantic_VarDec(struct Node *vardec, struct Type *type);
struct Symbol *semantic_FunDec(struct Node *fundec, struct Type *type);


void semantic_analysis()
{
    initializeSymbol();
    initializeType();
    struct Node *extdeflist = grammarTreeRoot->children[0];
    while (extdeflist->num_children != 0) {
        struct Node *extdef = extdeflist->children[0];       // ExtDefList -> ExtDef ExtDefList | null.
        if (strcmp(extdef->children[1]->type, "ExtDecList\0") == 0) {
            semantic_global_variable(extdef);                // ExtDef -> Specifier ExtDecList SEMI
        }
        else if (strcmp(extdef->children[1]->type, "SEMI\0") == 0) {
            semantic_structure_definition(extdef);           // ExtDef -> Specifier SEMI
        }
        else if (strcmp(extdef->children[1]->type, "FunDec\0") == 0)  {
            semantic_function_definition(extdef);            // ExtDef -> Specifier FunDec CompSt
        }
        else {
            printf("Error! In \'semantic_analysis\'. Debugging needed.\n");
        }
        extdeflist = extdeflist->children[1];
    }
    return;
}

void semantic_global_variable(struct Node *extdef)
{
    struct Type *type = semantic_Specifier(extdef->children[0]);
    if (type == NULL) return;
    semantic_ExtDecList(extdef->children[1], type);
}

void semantic_structure_definition(struct Node *extdef)
{
    semantic_Specifier(extdef->children[0]);
}

void semantic_function_definition(struct Node* extdef)
{
    struct Type *type = semantic_Specifier(extdef->children[0]);
    if (type == NULL) return;
    struct Symbol *func = semantic_FunDec(extdef->children[1], type);

    // to be continued
}

void semantic_ExtDecList(struct Node *extdeclist, struct Type *type)
{                                                   // ExtDecList -> VarDec | VarDec COMMA ExtDecList
    while (extdeclist != NULL) {
        struct Node *vardec = extdeclist->children[0];
        semantic_VarDec(vardec, type);
        if (extdeclist->num_children == 3) extdeclist = extdeclist->children[2];
        else break;
    }
}

struct Symbol *semantic_FunDec(struct Node *fundec, struct Type *type)
{
    struct Symbol *func;
    if (fundec->num_children == 3) {
        func = add_function(fundec->children[0]->type_string, type, NULL);
        if (func == NULL) {
            printf("Error type 4 at Line %d: Redefined function \"%s\".\n", 
                    fundec->line, fundec->children[0]->type_string);
        }
    }
    else {
        struct Symbol *paramsHead = NULL;
        struct Node *varlist = fundec->children[2];
        while (varlist != NULL) {
            struct Node *paramdec = varlist->children[0];
            struct Type *t = semantic_Specifier(paramdec->children[0]);
            struct Symbol *sym = semantic_VarDec(paramdec->children[1], t);
            if (sym == NULL) return NULL;
            sym->params = paramsHead;
            paramsHead = sym;
            if (varlist->num_children == 3) varlist = varlist->children[2];
            else break;
        }
        func = add_function(fundec->children[0]->type_string, type, paramsHead);
    }
    return func;
}

void semantic_CompSt(struct Node *compst)
{
    //to be continued
}

void semantic_DefList(struct Node *deflist)
{
    for (; deflist->num_children != 0; deflist = deflist->children[1]) {
        struct Node *def = deflist->children[0];
        struct Type *type = semantic_Specifier(def->children[0]);
        struct Node *declist = def->children[1];
        while (declist != NULL) {
            struct Node *dec = declist->children[0];
            struct Node *vardec = dec->children[0];
            struct Symbol *sym = semantic_VarDec(vardec, type);
            if (sym != NULL && dec->num_children == 3) {
                //to be continued
            } 
            if (declist->num_children == 3) declist = declist->children[2];
            else break;
        }
    }
}

struct Symbol *semantic_VarDec(struct Node *vardec, struct Type *type)
{
    if (vardec->num_children == 1) {
        struct Symbol *sym = add_variable(vardec->children[0]->type_string, type);
        if (sym == NULL) {
            printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",
                    vardec->line, vardec->children[0]->type_string);
        }
        return sym;
    }
    else {
        struct Type *arrayType = (struct Type *)malloc(sizeof(struct Type));
        arrayType->kind = ARRAY;
        arrayType->array.eleType = type;
        arrayType->array.size = vardec->children[2]->type_int;
        return semantic_VarDec(vardec->children[0], arrayType);
    }
}

struct Exp semantic_Exp(struct Node *exp)
{
    struct Exp e;
    if (exp->num_children == 1) {
        if (strcmp(exp->children[0]->type, "INT\0") == 0) {
            e.type = typeInt;
            e.var = false;
            return e;
        }
        else if (strcmp(exp->children[0]->type, "FLOAT\0") == 0) {
            e.type = typeFloat;
            e.var = false;
            return e;
        }
        else if (strcmp(exp->children[0]->type, "ID\0") == 0) {
            struct Symbol *sym = search_variable(exp->children[0]->type_string);
            if (sym == NULL) {
                printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", 
                        exp->line, exp->children[0]->type_string);
                e.type = NULL;
            }
            e.type = sym->structure;
            e.var = true;
            return e;
        }
    }
    else if (strcmp(exp->children[1]->type, "AND\0") == 0 || strcmp(exp->children[1]->type, "OR\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[0]);
        struct Exp e2 = semantic_Exp(exp->children[2]);
        if (e1.type == NULL || e2.type == NULL) {
            e.type = NULL;
            return e;
        }
        if (e1.type->kind != INTtype || e2.type->kind != INTtype) {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", exp->line);
            e.type = NULL;
            return e;
        }
        e.type = typeInt;
        e.var = false;
        return e;
    }
    else if (strcmp(exp->children[1]->type, "RELOP\0") == 0 ||
             strcmp(exp->children[1]->type, "PLUS\0") == 0 ||
             strcmp(exp->children[1]->type, "MINUS\0") == 0 ||
             strcmp(exp->children[1]->type, "STAR\0") == 0 ||
             strcmp(exp->children[1]->type, "DIV\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[0]);
        struct Exp e2 = semantic_Exp(exp->children[2]);
        if (e1.type == NULL || e2.type == NULL) {
            e.type = NULL;
            return e;
        }
        if ((e1.type->kind == INTtype && e2.type->kind == INTtype) || 
            (e1.type->kind == FLOATtype && e2.type->kind == FLOATtype)) {
            e.type = e1.type;
            e.var = false;
            return e;
        }
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", exp->line);
        e.type = NULL;
        return e;
    }
    else if (strcmp(exp->children[0]->type, "LP\0") == 0) {
        e = semantic_Exp(exp->children[1]);
        e.var = false;
        return e;
    }
    else if (strcmp(exp->children[0]->type, "MINUS\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[1]);
        if (e1.type == NULL) {
            e.type = NULL;
            return e;
        }
        if (e1.type->kind != INTtype && e1.type->kind != FLOATtype) {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", exp->line);
            e.type = NULL;
            return e;
        }
        e.type = e1.type;
        e.var = false;
        return e;
    }
    else if (strcmp(exp->children[0]->type, "NOT\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[1]);
        if (e1.type == NULL) {
            e.type = NULL;
            return e;
        }
        if (e1.type->kind != INTtype) {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", exp->line);
            e.type = NULL;
            return e;
        }
        e.type = typeInt;
        e.var = false;
        return e;
    }
    else if (strcmp(exp->children[1]->type, "LB\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[0]);
        struct Exp e2 = semantic_Exp(exp->children[2]);
        if (e1.type == NULL) {
            e.type = NULL;
            return e;
        }
        if (e1.type->kind != ARRAY) {
            printf("Error type 10 at Line %d: Not an array.\n", exp->line);
            e.type = NULL;
            return e;
        }
        if (e2.type == NULL) {
            e.type = NULL;
            return e;
        }
        if (e2.type->kind != INTtype) {
            printf("Error type 12 at Line %d: Not an integer.\n", exp->line);
            e.type = NULL;
            return e;
        }
        e.type = e1.type->array.eleType;
        e.var = e1.var;
        return e;
    }
    else if (strcmp(exp->children[1]->type, "DOT\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[0]);
        if (e1.type == NULL) {
            e.type = NULL;
            return e;
        }
        if (e1.type->kind != STRUCTURE) {
            printf("Error type 13 at Line %d: Illegal use of \".\".\n", exp->line);
            e.type = NULL;
            return e;
        }
        char *fieldName = exp->children[2]->type_string;
        struct FieldList *field = e1.type->structure;
        for (; field != NULL; field = field->next) {
            if (strcmp(field->name, fieldName) == 0) {
                break;
            }
        }
        if (field == NULL) {
            printf("Error type 13 at Line %d: Non-existent field \"%s\".\n", exp->line, fieldName);
            e.type = NULL;
            return e;
        }
        e.type = field->fieldType;
        e.var = e1.var;
        return e;
    }
    /* 这边写不出来啊啊啊啊啊！！！ */
    else if (strcmp(exp->children[1]->type, "LP\0") == 0) {
        struct Symbol *func = search_function(exp->children[0]->type_string);
        if (func == NULL) {
            printf("Error type 2 at Line %d: Undefined function \"%s\".\n", exp->line, exp->children[0]->type_string);
            e.type = NULL;
            return e;
        }
        if (strcmp(exp->children[2]->type, "RP\0") == 0) {
            e.type = func->structure;
            e.var = false;
            return e;
        }
        else {
            struct Node *Args = exp->children[2];
            while (Args != NULL) {
                struct Node *exp_new = Args->children[0];
                struct Exp e_new = semantic_Exp(exp_new);
                if (e_new.type == NULL) {
                    e.type = NULL;
                    return e;
                }
                /* ************ To Be Continued ************ */

                if (Args->num_children == 3) Args = Args->children[2];
                else break;
            }
            e.type = func->structure;
            e.var = false;
            return e;
        }
    }
    else if (strcmp(exp->children[1]->type, "ASSIGNOP\0") == 0) {
        struct Exp e1 = semantic_Exp(exp->children[0]);
        struct Exp e2 = semantic_Exp(exp->children[2]);
        if (e1.type == NULL || e2.type == NULL) {
            e.type = NULL;
            return e;
        }
        /* 这边使用了名等价啊啊啊啊啊啊！！！！ */
        /* 先判断是否可赋值，再判断是否同类型的可以赋值。 */
        if (e1.var == false) {
            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.", exp->line);
            e.type = NULL;
            return e;
        }
        if (e1.type != e2.type) {
            printf("Error type 5 at Line %d: Type mismatched for assignment.", exp->line);
            e.type = NULL;
            return e;
        }
        e.type = e1.type;
        e.var = e1.var;
        return e;
    }
    return e;
}
