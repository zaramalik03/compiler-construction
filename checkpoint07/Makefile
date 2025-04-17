#------------------------------------------------------------------------
# Name----------Zara Malik
# Course--------CS4223-01
# Assignment----Checkpoint #7: While and exit statements; mod and arrays
# Date----------April 14, 2025
# This is the Makefile that builds the application for Checkpoint #7.
#------------------------------------------------------------------------

ifeq ($(shell uname),Darwin)
	# For Macintosh
	FLEXLIB = -ll
else
	# For Linux, Cygwin, etc.
	FLEXLIB = -lfl
endif

a.out: scanner.l parser.y symbolTable.c ast.c symbolTable.h
	flex scanner.l
	bison -y -d parser.y
	gcc -o a.out lex.yy.c y.tab.c symbolTable.c ast.c ${FLEXLIB}


clean:
	rm -f core *.o lex.yy.c y.tab.c y.tab.h symbolTable.exe a.out
