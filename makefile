ifeq ($(shell uname), Darwin)
	LINK_FLAG = -ll
else
	LINK_FLAG = -lfl
endif

PARSER: lexical.l syntax.y main.c grammarTree.c grammarTree.h
	@bison -d syntax.y
	@flex lexical.l
	@gcc main.c syntax.tab.c grammarTree.c $(LINK_FLAG) -ly -o parser

clean:
	@rm -rf parser syntax.tab.c syntax.tab.h lex.yy.c