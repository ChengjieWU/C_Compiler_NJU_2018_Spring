#ifndef __GRAMMARTREE_H__
#define __GRAMMARTREE_H__

#define true 1
#define false 0
#define bool int

#define MAX_CHILDREN 10

struct Node {
    union {
        int type_int;
        float type_float;
        double type_double;
        char type_string[32];
    };
    int line;
    char type[32];
    int num_children;
    struct Node* children[MAX_CHILDREN];
    struct Node* parent;
    bool terminal;
};

struct Node* create_node(char type[], int line, bool terminal);
int create_link(struct Node* parent, struct Node* child);
void print_tree(struct Node* root, int whitespace);


#endif