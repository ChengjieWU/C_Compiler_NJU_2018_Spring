#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__

#include "grammarTree.h"

#define SYMBOL_T_NAME_MAX_LEN 32
#define MAX_CODE_LEN 128

enum sym_kind {SYM_VAR, SYM_FUNC, SYM_LABEL};
enum op_kind {OP_VAR, OP_CONSTANT, OP_FUNC, OP_LABEL};
enum ir_kind {IR_LABEL, IR_FUNCTION, IR_ASSIGN, IR_ADD,
IR_MINUS, IR_MUL, IR_DIV, IR_ADDRESS, IR_POINTL, IR_POINTR,
IR_GOTO, IR_GOL, IR_GOG, IR_GOLE, IR_GOGE, IR_GOE, IR_GONE, 
IR_RETURN, IR_DEC, IR_ARG, IR_CALL, IR_PARAM, IR_READ, IR_WRITE};

struct Symbol_t {
    enum sym_kind kind;
    char name[SYMBOL_T_NAME_MAX_LEN];
    struct Symbol_t *next;
    struct Symbol_t *arg_next;
};

struct Operand {
    enum op_kind kind;
    union {
        int value;
        struct Symbol_t *sym;
    };
};

struct InterCode {
    enum ir_kind kind;
    struct Operand op1;
    struct Operand op2;
    struct Operand result;
    struct InterCode *next;
};

struct CodeBlock {
    struct InterCode *begin;
    struct InterCode *end;
};
typedef struct CodeBlock CB;

void write_CB(CB cb, FILE *f);
CB translate_IR();

#endif