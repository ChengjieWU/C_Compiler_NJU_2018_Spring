#include <string.h>
#include <stdio.h>
#include "grammarTree.h"
#include "type.h"
#include "symbol.h"

extern struct Node* grammarTreeRoot;
void semantic_analysis();
void semantic_global_variable(struct Node *p);
void semantic_structure_definition(struct Node *p);
void semantic_function(struct Node *p);


void semantic_analysis()
{
    initializeSymbol();
    struct Node *p = grammarTreeRoot->children[0];      // p is ExtDefList
    while (p->num_children != 0) {
        struct Node *q = p->children[0];                // ExtDefList -> ExtDef ExtDefList | null. q is ExtDef.
        if (strcmp(q->children[1]->type, "ExtDecList\0") == 0) {
            semantic_global_variable(q);                // ExtDef -> Specifier ExtDecList SEMI
        }
        else if (strcmp(q->children[1]->type, "SEMI\0") == 0) {
            semantic_structure_definition(q);           // ExtDef -> Specifier SEMI
        }
        else if (strcmp(q->children[1]->type, "FunDec\0") == 0)  {
            semantic_function(q);                       // ExtDef -> Specifier FunDec CompSt
        }
        else {
            printf("Error! In \'semantic_analysis\'. Debugging needed.\n");
        }
        p = p->children[1];
    }
    return;
}

void semantic_global_variable(struct Node *p){}
void semantic_structure_definition(struct Node *p){}
void semantic_function(struct Node *p){}
