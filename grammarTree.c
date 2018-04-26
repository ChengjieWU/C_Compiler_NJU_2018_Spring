#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mm_malloc.h>
#include "grammarTree.h"


int create_link(struct Node* parent, struct Node* child)
{
    if (parent->num_children == MAX_CHILDREN-1) {
        return -1;
    }
    child->parent = parent;
    parent->children[parent->num_children] = child;
    parent->num_children += 1;
    return 0;
}


void print_tree(struct Node* root, int whitespace)
{
    if (root->terminal == false && root->num_children == 0) {
        return;
    }
    int i;
    for (i = 0; i < whitespace; i++) {
        putchar(' ');
    }
    if (root->terminal == false) {
        printf("%s (%d)\n", root->type, root->line);
        for (i = 0; i < root->num_children; i++) {
            print_tree(root->children[i], whitespace+2);
        }
    }
    else {
        if (strcmp(root->type, "ID\0") == 0 || strcmp(root->type, "TYPE\0") == 0) {
            printf("%s: %s\n", root->type, root->type_string);
        }
        else if (strcmp(root->type, "INT\0") == 0 
        || strcmp(root->type, "INT8\0") == 0 
        || strcmp(root->type, "INT16\0") == 0) {
            printf("INT: %d\n", root->type_int);
        }
        else if (strcmp(root->type, "FLOAT\0") == 0 || strcmp(root->type, "FLOATE\0") == 0) {
            printf("FLOAT: %f\n", root->type_float);
        }
        else {
            printf("%s\n", root->type);
        }
    }
    return;
}


struct Node* create_node(char type[], int line, bool terminal)
{
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));
    p->line = line;
    strcpy(p->type, type);
    p->num_children = 0;
    int i;
    for (i = 0; i < MAX_CHILDREN; i++) {
        p->children[i] = NULL;
    }
    p->parent = NULL;
    p->terminal = terminal;
    return p;
}
