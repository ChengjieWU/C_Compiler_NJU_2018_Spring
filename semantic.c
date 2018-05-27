#include <string.h>
#include <stdio.h>
#include <mm_malloc.h>
#include "symbol.h"
#include "semantic.h"

void semantic_analysis();
void semantic_global_variable(struct Node *extdef);
void semantic_structure_definition(struct Node *extdef);
void semantic_function_definition(struct Node* extdef);
void semantic_ExtDecList(struct Node *extdeclist, struct Type *type);
struct Symbol *semantic_FunDec(struct Node *fundec, struct Type *type);
void semantic_CompSt(struct Node *compst, struct Symbol *func);
void semantic_DefList(struct Node *deflist);
struct Symbol *semantic_VarDec(struct Node *vardec, struct Type *type);
struct Exp semantic_Exp(struct Node *exp);
void semantic_StmtList(struct Node *stmtlist, struct Symbol *func);
void semantic_Stmt(struct Node *stmt, struct Symbol *func);
struct Parameters *get_parameterType_list(struct Symbol *func);
void delete_parameterType_list(struct Parameters *head);

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
    if (func == NULL) return;
    semantic_CompSt(extdef->children[2], func);
    return;
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

void semantic_CompSt(struct Node *compst, struct Symbol *func)
{
    semantic_DefList(compst->children[1]);
    semantic_StmtList(compst->children[2], func);
    return;
}

/* 若变量成功定义，但初始化失败，那么变量是不删除的。 */
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
                struct Exp e = semantic_Exp(dec->children[2]);
                /* 若变量成功定义，但初始化失败，那么变量是不删除的。 */
                if (compare_type(e.type, sym->structure) == false) {
                    printf("Error type 5 at Line %d: Type mismatched for assignment.\n", dec->line);
                }
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
    /* Exp -> ID | INT | FLOAT */
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
                return e;
            }
            e.type = sym->structure;
            e.var = true;
            return e;
        }
    }
    /* Exp -> Exp AND Exp | Exp OR Exp */
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
            printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", exp->line, fieldName);
            e.type = NULL;
            return e;
        }
        e.type = field->fieldType;
        e.var = e1.var;
        return e;
    }
    /* Exp -> ID LP Args RP | ID LP RP */
    else if (strcmp(exp->children[1]->type, "LP\0") == 0) {
        struct Symbol *func = search_symbol(exp->children[0]->type_string);
        if (func == NULL) {
            printf("Error type 2 at Line %d: Undefined function \"%s\".\n", exp->line, exp->children[0]->type_string);
            e.type = NULL;
            return e;
        }
        if (func->kind != FUNCTION) {
            printf("Error type 11 at Line %d: \"%s\" is not a function.\n", exp->line, exp->children[0]->type_string);
            e.type = NULL;
            return e;
        }
        if (strcmp(exp->children[2]->type, "RP\0") == 0) {
            struct Parameters *par = get_parameterType_list(func);
            if (par != NULL) {
                printf("Error type 9 at Line %d: Invalid arguments.\n", exp->line);
                e.type = NULL;
                return e;
            }
            e.type = func->structure;
            e.var = false;
            return e;
        }
        else {
            struct Parameters *par = get_parameterType_list(func);
            struct Parameters *parIter = par;
            struct Node *Args = exp->children[2];
            while (parIter != NULL) {
                struct Node *exp_new = Args->children[0];
                struct Exp e_new = semantic_Exp(exp_new);
                if (e_new.type == NULL) {
                    e.type = NULL;
                    return e;
                }
                if (compare_type(e_new.type, parIter->type) == false) {
                    printf("Error type 9 at Line %d: Invalid arguments.\n", exp_new->line);
                    e.type = NULL;
                    return e;
                }
                parIter = parIter->next;
                if (parIter == NULL) break;
                if (Args->num_children == 3) Args = Args->children[2];
                else break;
            }
            if (parIter != NULL || Args->num_children == 3) {
                printf("Error type 9 at Line %d: Invalid arguments.\n", Args->line);
                e.type = NULL;
                return e;
            }
            delete_parameterType_list(par);
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
        /* 先判断是否可赋值，再判断是否同类型的可以赋值。 */
        if (e1.var == false) {
            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", exp->line);
            e.type = NULL;
            return e;
        }
        if (compare_type(e1.type, e2.type) == false) {
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n", exp->line);
            e.type = NULL;
            return e;
        }
        e.type = e1.type;
        e.var = e1.var;
        return e;
    }
    return e;
}

void semantic_StmtList(struct Node *stmtlist, struct Symbol *func)
{
    for (; stmtlist->num_children != 0; stmtlist = stmtlist->children[1]) {
        struct Node *stmt = stmtlist->children[0];
        semantic_Stmt(stmt, func);
    }
    return;
}

void semantic_Stmt(struct Node *stmt, struct Symbol *func)
{
    if (strcmp(stmt->children[0]->type, "Exp\0") == 0) {
        semantic_Exp(stmt->children[0]);
    }
    else if (strcmp(stmt->children[0]->type, "CompSt\0") == 0) {
        semantic_CompSt(stmt->children[0], NULL);
    }
    else if (strcmp(stmt->children[0]->type, "RETURN\0") == 0) {
        struct Exp e = semantic_Exp(stmt->children[1]);
        if (e.type == NULL) {
            return;
        }
        if (compare_type(func->structure, e.type) == false) {
            printf("Error type 8 at Line %d: Type mismatched for return.\n", stmt->line);
        }
    }
    else if (strcmp(stmt->children[0]->type, "WHILE\0") == 0) {
        struct Exp e = semantic_Exp(stmt->children[2]);
        if (e.type == NULL) {
            return;
        }
        if (e.type->kind != INTtype) {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", stmt->children[2]->line);
            return;
        }
        semantic_Stmt(stmt->children[4], NULL);
    }
    else {
        struct Exp e = semantic_Exp(stmt->children[2]);
        if (e.type == NULL) {
            return;
        }
        if (e.type->kind != INTtype) {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", stmt->children[2]->line);
            return;
        }
        semantic_Stmt(stmt->children[4], NULL);
        if (stmt->num_children == 7) {
            semantic_Stmt(stmt->children[6], NULL);
        }
    }
    return;
}

struct Parameters *get_parameterType_list(struct Symbol *func)
{
    struct Parameters *head = NULL;
    struct Parameters *current = NULL;
    if (func->kind != FUNCTION) {
        printf("the symbol is not a function\n");
        return NULL;
    }
    struct Symbol *sym = func->params;
    for (; sym != NULL; sym = sym->params) {
        struct Parameters *p = (struct Parameters *)malloc(sizeof(struct Parameters));
        p->type = sym->structure;
        p->next = NULL;
        if (head == NULL) {
            head = p;
            current = p;
        }
        else {
            current->next = p;
            current = p;
        }
    }
    return head;
}

void delete_parameterType_list(struct Parameters *head)
{
    while (head != NULL) {
        struct Parameters *re = head;
        head = head->next;
        free(re);
    }
    return;
}