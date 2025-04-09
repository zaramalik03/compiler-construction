#--------------------------------------------------------------------
# Name----------Zara Malik
# Course--------CS4223-01
# Assignment----Checkpoint #3- Parsing Data Section
# Date----------February 14, 2025
# This is the Makefile that builds the application for Checkpoint #3.
#--------------------------------------------------------------------

ifeq ($(shell uname),Darwin)
	# For Macintosh
	FLEXLIB = -ll
else
	# For Linux, Cygwin, etc.
	FLEXLIB = -lfl
endif

a.out: scanner.l parser.y main.c
	bison -y -d parser.y
	flex scanner.l
	gcc main.c lex.yy.c y.tab.c ${FLEXLIB}

clean:
	rm -f a.out y.tab.c y.tab.h lex.yy.c
