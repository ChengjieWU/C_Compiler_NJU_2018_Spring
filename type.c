#include <mm_malloc.h>
#include <string.h>
#include <stdio.h>
#include "type.h"
#include "symbol.h"

struct Type *semantic_specifier(struct Node *p)            // p is Specifier
{
    struct Type *type;
    p = p->children[0];                             // p is child of original p
    if (strcmp(p->type, "TYPE\0") == 0) {           // Specifier -> TYPE
        type = (struct Type *)malloc(sizeof(struct Type));
        if (strcmp(p->type_string, "int\0") == 0) {
            type->kind = INTtype;
        }
        else if (strcmp(p->type_string, "float\0") == 0) {
            type->kind = FLOATtype;
        }
        else printf("Error in \'semantic_specifier\'!\n");
    }
    else if (strcmp(p->type, "StructSpecifier\0") == 0) {   // Specifier -> StructSpecifier
        if (p->num_children == 2) {                         // StructSpecifier -> STRUCT Tag
            struct Type* type = search_structure(p->children[1]->children[0]->type_string);
        }
        else printf("Error in \'semantic_specifier\'!\n");
    }
    else printf("Error in \'semantic_specifier\'!\n");
    return type;
}
