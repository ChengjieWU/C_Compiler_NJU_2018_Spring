#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "type.h"

struct Exp {
    struct Type *type;
    bool var;
};

struct Parameters {
    struct Type *type;
    struct Parameters *next;
};


void semantic_analysis();


#endif