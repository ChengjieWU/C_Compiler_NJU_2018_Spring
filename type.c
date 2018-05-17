#include <mm_malloc.h>
#include <string.h>
#include <stdio.h>
#include "type.h"
#include "symbol.h"

/*
    1. 回收未使用的type
*/

void print_type(struct Type *type)
{
    switch (type->kind) {
        case INTtype:
            printf("INT\t");
            break;
        case FLOATtype:
            printf("FLOAT\t");
            break;
        case ARRAY:
            printf("Array\t");
            printf("%d\t", type->array.size);
            print_type(type->array.eleType);
            break;
        case STRUCTURE:
            printf("Structure\t");
            struct FieldList *p = type->structure;
            for (; p != NULL; p = p->next) {
                printf("field %s\t", p->name);
                print_type(p->fieldType);
            }
            break;
        default:
            break;
    }
}

struct Type *semantic_Specifier(struct Node *specifier)            // p is Specifier
{
    if (strcmp(specifier->children[0]->type, "TYPE\0") == 0) {                   // Specifier -> TYPE
        struct Type *type = (struct Type *)malloc(sizeof(struct Type));
        if (strcmp(specifier->children[0]->type_string, "int\0") == 0) {
            type->kind = INTtype;
        }
        else if (strcmp(specifier->children[0]->type_string, "float\0") == 0) {
            type->kind = FLOATtype;
        }
        else printf("Error in \'semantic_specifier\'!\n");
        return type;
    }
    else if (strcmp(specifier->children[0]->type, "StructSpecifier\0") == 0) {   // Specifier -> StructSpecifier
        struct Node *structspecifier = specifier->children[0];
        if (structspecifier->num_children == 2) {                                // StructSpecifier -> STRUCT Tag
            struct Type *type = search_structure(structspecifier->children[1]->children[0]->type_string);
            if (type == NULL) {
                printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", 
                        structspecifier->line, structspecifier->children[1]->children[0]->type_string);
            }
            return type;
        }
        else {                                                        // StructSpecifier -> STRUCT OptTag LC DefList RC
            if (structspecifier->children[1]->num_children != 0 &&    // 非匿名定义，并且结构名已经被使用过了，报错！
                    search_structure(structspecifier->children[1]->children[0]->type_string) != NULL) {
                printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", 
                        structspecifier->line, structspecifier->children[1]->children[0]->type_string);
                return NULL;
            }
            else {                                                        // define a new structure
                struct Type *type = (struct Type *)malloc(sizeof(struct Type));
                type->kind = STRUCTURE;
                type->structure = NULL;
                struct Node *deflist = structspecifier->children[3];
                while (deflist->num_children != 0) {                        // DefList -> Def DefList | NULL
                    struct Node *def = deflist->children[0];                // Def -> Specifier DecList SEMI
                    struct Type *ori_type = semantic_Specifier(def->children[0]);
                    if (ori_type == NULL) return NULL;     // return NULL if error occurs in fields

                    struct Node *declist = def->children[1];

                    while (declist->num_children == 3) {                // DecList -> Dec COMMA DecList | Dec
                        struct Node *dec = declist->children[0];            // Dec -> VarDec | VarDec ASSIGNOP Exp
                        if (dec->num_children != 1) {
                            printf("Error type 15 at Line %d: Initialized field.\n", dec->line);
                            return NULL;
                        }
                        struct Node *vardec = dec->children[0];
                        struct Type *field_type = semantic_VarDec_inStruct(vardec, ori_type);
                        if (field_type == NULL) {
                            printf("Error type 15 at Line %d: Redefined field \"%s\".\n", 
                                    vardec->line, rootHead->name);
                            return NULL;
                        }
                        struct FieldList *field = (struct FieldList *)malloc(sizeof(struct FieldList));
                        strcpy(field->name, rootHead->name);            // 用此时符号表最上层符号名字记录下来
                        field->fieldType = field_type;
                        field->next = type->structure;
                        type->structure = field;
                        declist = declist->children[2];
                    }
                    
                    struct Node *dec = declist->children[0];            // Dec -> VarDec | VarDec ASSIGNOP Exp
                    if (dec->num_children != 1) {
                        printf("Error type 15 at Line %d: Initialized field.\n", dec->line);
                        return NULL;
                    }
                    struct Node *vardec = dec->children[0];
                    struct Type *field_type = semantic_VarDec_inStruct(vardec, ori_type);
                    if (field_type == NULL) {
                        return NULL;
                    }
                    struct FieldList *field = (struct FieldList *)malloc(sizeof(struct FieldList));
                    strcpy(field->name, rootHead->name);        // 用此时符号表最上层符号名字记录下来
                    field->fieldType = field_type;
                    field->next = type->structure;
                    type->structure = field;

                    deflist = deflist->children[1];
                }
                if (structspecifier->children[1]->num_children != 0) {        // 若不是匿名struct，则加入符号表
                    add_structure(structspecifier->children[1]->children[0]->type_string, type);
                }
                return type;
            }
        }
    }
    else printf("Error in \'semantic_specifier\'!\n");
    return NULL;
}

struct Type *semantic_VarDec_inStruct(struct Node *vardec, struct Type *type)
{
    if (vardec->num_children == 1) {
        int ret = add_variable(vardec->children[0]->type_string, type);
        if (ret == -1) {
            printf("Error type 15 at Line %d: Redefined field \"%s\".\n", 
                    vardec->line, vardec->children[0]->type_string);
            return NULL;
        }
        return type;
    }
    else {
        struct Type *arrayType = (struct Type *)malloc(sizeof(struct Type));
        arrayType->kind = ARRAY;
        arrayType->array.eleType = type;
        arrayType->array.size = vardec->children[2]->type_int;
        return semantic_VarDec_inStruct(vardec->children[0], arrayType);
    }
}
