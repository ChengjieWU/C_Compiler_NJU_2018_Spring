#include <string.h>
#include <stdio.h>
#include "grammarTree.h"
#include "type.h"
#include "symbol.h"

void semantic_analysis();
void semantic_global_variable(struct Node *extdef);
void semantic_structure_definition(struct Node *extdef);
void semantic_function(struct Node *extdef);


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
            semantic_function(extdef);                       // ExtDef -> Specifier FunDec CompSt
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
    
}

void semantic_structure_definition(struct Node *extdef)
{
    semantic_Specifier(extdef->children[0]);
}

void semantic_function(struct Node *extdef){}
