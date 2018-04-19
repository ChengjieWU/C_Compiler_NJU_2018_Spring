PARSER: lexical.l syntax.y main.c grammarTree.c grammarTree.h
	@bison -d syntax.y
	@flex lexical.l
	@gcc main.c syntax.tab.c grammarTree.c -ll -ly -o parser

clean:
	@rm -rf parser syntax.tab.c syntax.tab.h lex.yy.c