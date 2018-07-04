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
void load_register_symbol(struct Symbol_t *sym, int reg, FILE *f);
void store_register_symbol(struct Symbol_t *sym, int reg, FILE *f);
int calculate_offset();
int lookup_offset(char *name);
int reg_load_operand(struct Operand op, FILE *f);
void dump_register(int reg, FILE* f);
int link_symbol_register(struct Symbol_t *sym, FILE *f); 
void print_regs();


struct Register
{
    bool free;
    bool value;
    struct Symbol_t *sym;
};

struct Register regs[MAX_REGS];

#endif