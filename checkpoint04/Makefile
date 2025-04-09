#--------------------------------------------------------------------
# Name----------Zara Malik
# Course--------CS4223-01
# Assignment----Checkpoint #4 - Constructing Symbol Table
# Date----------March 5, 2025
# This is the Makefile that builds the application for Checkpoint #4.
#--------------------------------------------------------------------

ifeq ($(shell uname),Darwin)
	# For Macintosh
	FLEXLIB = -ll
else
	# For Linux, Cygwin, etc.
	FLEXLIB = -lfl
endif

symbolTable: scanner.l parser.y main.c symbolTable.h
	flex scanner.l
	bison -y -d parser.y
	gcc -o symbolTable main.c lex.yy.c y.tab.c ${FLEXLIB}


clean:
	rm -f core *.o lex.yy.c y.tab.c y.tab.h symbolTable.exe symbolTable
