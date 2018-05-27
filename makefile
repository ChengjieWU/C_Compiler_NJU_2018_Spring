ifeq ($(shell uname), Darwin)
	LINK_FLAG = -ll
	CC = gcc-8
else
	LINK_FLAG = -lfl
	CC = gcc
endif

PARSER: lexical.l syntax.y main.c grammarTree.c type.c symbol.c semantic.c grammarTree.h type.h symbol.h semantic.h
	@bison -d syntax.y
	@flex lexical.l
	@$(CC) main.c syntax.tab.c grammarTree.c type.c symbol.c semantic.c $(LINK_FLAG) -ly -o parser

clean:
	@rm -rf parser syntax.tab.c syntax.tab.h lex.yy.c
