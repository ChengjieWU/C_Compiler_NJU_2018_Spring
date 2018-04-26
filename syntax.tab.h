/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     FLOAT = 259,
     ASSIGNOP = 260,
     RELOP = 261,
     PLUS = 262,
     MINUS = 263,
     STAR = 264,
     DIV = 265,
     AND = 266,
     OR = 267,
     NOT = 268,
     TYPE = 269,
     STRUCT = 270,
     LC = 271,
     RC = 272,
     LB = 273,
     RB = 274,
     LP = 275,
     RP = 276,
     SEMI = 277,
     COMMA = 278,
     ID = 279,
     RETURN = 280,
     IF = 281,
     ELSE = 282,
     WHILE = 283,
     DOT = 284,
     LOWER_THAN_ELSE = 285
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define ASSIGNOP 260
#define RELOP 261
#define PLUS 262
#define MINUS 263
#define STAR 264
#define DIV 265
#define AND 266
#define OR 267
#define NOT 268
#define TYPE 269
#define STRUCT 270
#define LC 271
#define RC 272
#define LB 273
#define RB 274
#define LP 275
#define RP 276
#define SEMI 277
#define COMMA 278
#define ID 279
#define RETURN 280
#define IF 281
#define ELSE 282
#define WHILE 283
#define DOT 284
#define LOWER_THAN_ELSE 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 9 "syntax.y"
{
    int type_int;
    float type_float;
    double type_double;
    char type_string[32];
    struct Node* type_node_pointer;
}
/* Line 1529 of yacc.c.  */
#line 117 "syntax.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
