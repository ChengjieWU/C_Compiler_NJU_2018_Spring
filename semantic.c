#include <string.h>
#include <stdio.h>
#include <mm_malloc.h>
#include "grammarTree.h"
#include "type.h"
#include "symbol.h"

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
    struct Node *extdeflist = grammarTreeRoot->children[0];      // p is ExtDefList
    while (extdeflist->num_children != 0) {
        struct Node *extdef = extdeflist->children[0];                // ExtDefList -> ExtDef ExtDefList | null. q is ExtDef.
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

struct Symbol *semantic_VarDec(struct Node *vardec, struct Type *type)
{
    struct Symbol *sym;
    if (vardec->num_children == 1) {
        sym = add_variable(vardec->children[0]->type_string, type);
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
