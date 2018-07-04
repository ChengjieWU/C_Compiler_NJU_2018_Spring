ifeq ($(shell uname), Darwin)
	LINK_FLAG = -ll
	CC = gcc-8
else
	LINK_FLAG = -lfl
	CC = gcc
endif

PARSER: lexical.l syntax.y main.c grammarTree.c type.c symbol.c semantic.c translate.c codeGeneration.c grammarTree.h type.h symbol.h semantic.h translate.h codeGeneration.h
	@bison -d syntax.y
	@flex lexical.l
	@$(CC) main.c syntax.tab.c grammarTree.c type.c symbol.c semantic.c translate.c codeGeneration.c $(LINK_FLAG) -ly -o parser -w

clean:
	@rm -rf parser syntax.tab.c syntax.tab.h lex.yy.c
