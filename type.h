#ifndef __TYPE_H__
#define __TYPE_H__

#include "grammarTree.h"

#define FILED_NAME_MAX_LEN 32

enum typeKind {INTtype, FLOATtype, ARRAY, STRUCTURE};

struct Type {
    enum typeKind kind;
    union {
        struct {
            struct Type *eleType;
            int size;
        } array;
        struct FieldList *structure;
    };
};

struct FieldList {
    char name[FILED_NAME_MAX_LEN];
    struct Type *fieldType;
    struct FieldList *next;
};

struct Type *typeInt;           // Global constant int type
struct Type *typeFloat;         // Global constant float type

void initializeType();
void print_type(struct Type *type);
struct Type *semantic_Specifier(struct Node *specifier);
struct Type *semantic_VarDec_inStruct(struct Node *vardec, struct Type *stc);
bool compare_type(struct Type *t1, struct Type *t2);

#endif