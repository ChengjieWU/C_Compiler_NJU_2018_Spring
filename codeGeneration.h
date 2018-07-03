#ifndef __CODEGENERATION_H__
#define __CODEGENERATION_H__

#include <stdio.h>
#include "translate.h"

#define FILE_LINE_BUFFER 128

#define MAX_REGS 10

void generate_text(CB ic, FILE *f);
bool generate_components(FILE *f);
bool copy_from_file(char source_file[], FILE *f);
void generate_code(CB ic, FILE *f);
void initialize_code_generation();
void load_operand(struct Operand op, int reg, FILE *f);
void load_register_symbol(struct Symbol_t *sym, int reg, FILE *f);
void store_register_symbol(struct Symbol_t *sym, int reg, FILE *f);
int calculate_offset();

struct Register
{
    bool free;
    struct Symbol_t *sym;
};

struct Register regs[MAX_REGS];

#endif