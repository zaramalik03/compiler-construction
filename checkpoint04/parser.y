%{

/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #4 - Constructing Symbol Table
 * Date----------March 5, 2025
 * This is the bison parser file that includes a complete data section and
 * an empty algorithm section. It calls the lookup and insert functions to 
 * output the correct contents of the symbol table.
 *-------------------------------------------------------------------------
 */

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"

int yyerror(const char *msg);
int yylex();
void show();


%}

%union {
   char *sval;
   int ival;
   double rval;
}

%token        MAIN_KW
%token        DATA_KW
%token        ALGORITHM_KW
%token        INTEGER_KW  
%token        REAL_KW
%token        WHILE_KW
%token        COUNTING_KW
%token        UPWARD_KW
%token        DOWNWARD_KW
%token        TO_KW
%token        IF_KW
%token        ELSE_KW
%token        PRINT_KW
%token        READ_KW
%token        END_KW  
%token        EXIT_KW
%token <sval> ID
%token <ival> INTEGER_CONST
%token <rval> REAL_CONST
%token <sval> STRING_CONST
%token        NEWLINE
%token        SEMICOLON
%token        COLON
%token        COMMA
%token        ASSIGN
%token        PLUS
%token        MINUS
%token        TIMES
%token        DIVIDE
%token        MODULUS
%token        LT
%token        LE
%token        GT
%token        GE
%token        EQ
%token        NE
%token        AND
%token        OR
%token        NOT
%token        LPAREN
%token        RPAREN
%token        LBRACKET
%token        RBRACKET
%token        EXCLAMATION
%token        TRASH


%%
program: 
    main_statement data_section algorithm_section end_main_statement{
        show();
    }
    ;

main_statement:      
    MAIN_KW SEMICOLON {
        initialize();
    }
    ;

data_section:        
    DATA_KW COLON declarations
    | DATA_KW COLON /* can be empty */
    ;

declarations:
    declaration
    | declarations declaration /* recursing if there is more than one declaration */
    ;    

declaration:
    INTEGER_KW COLON int_var_list SEMICOLON  /* uses either integer or real keywords */
    | REAL_KW COLON real_var_list SEMICOLON
    ;

int_var_list:
    ID {
        lookup($1);
        if (!lookup($1)) yyerror($1);
        else insert($1, DT_INTEGER, VK_SCALAR, 1);
    }
    | ID LBRACKET INTEGER_CONST RBRACKET {
        lookup($1);
        if (!lookup($1)) yyerror($1);
        else insert($1, DT_INTEGER, VK_ARRAY, $3);
    }
    | int_var_list COMMA ID {
        lookup($3);
        if (!lookup($3)) yyerror($3);
        else insert($3, DT_INTEGER, VK_SCALAR, 1);
    }
    | int_var_list COMMA ID LBRACKET INTEGER_CONST RBRACKET {
        lookup($3);
        if (!lookup($3)) yyerror($3);
        else insert($3, DT_INTEGER, VK_ARRAY, $5);
    }
    ;

real_var_list:
    ID {
        lookup($1);
        if (!lookup($1)) yyerror($1);
        else insert($1, DT_REAL, VK_SCALAR, 1);
    }
    | ID LBRACKET REAL_CONST RBRACKET {
        lookup($1);
        if (!lookup($1)) yyerror($1);
        else insert($1, DT_REAL, VK_ARRAY, $3);
    }
    | real_var_list COMMA ID {
        lookup($3);
        if (!lookup($3)) yyerror($3);
        else insert($3, DT_REAL, VK_SCALAR, 1);
    }
    | real_var_list COMMA ID LBRACKET REAL_CONST RBRACKET {
        lookup($3);
        if (!lookup($3)) yyerror($3);
        else insert($3, DT_REAL, VK_ARRAY, $5);
    }
    ;


algorithm_section:
    ALGORITHM_KW COLON /* empty */
    ;

end_main_statement:  
    END_KW MAIN_KW SEMICOLON
    ; 

%%

int yyerror(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    return 1;
}
