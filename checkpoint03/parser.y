%{

/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #3- Parsing Data Section
 * Date----------February 14, 2025
 * This is the bison parser file that includes a complete data section and
 * an empty algorithm section. The necessary tokens and declarations are 
 * needed for this to parse successfully.
 *-------------------------------------------------------------------------
 */

#include "y.tab.h"
#include <stdio.h>

int yyerror();
int yylex();

%}

%union {
   char *sval;
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
%token        READ_LW
%token        END_KW  
%token <sval> ID
%token <sval> INTEGER_CONST
%token <sval> REAL_CONST
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
    main_statement data_section algorithm_section end_main_statement
    ;

main_statement:      
    MAIN_KW SEMICOLON 
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
    INTEGER_KW COLON var_list SEMICOLON  /* uses either integer or real keywords */
    | REAL_KW COLON var_list SEMICOLON
    ;

var_list:
    ID
    | ID LBRACKET INTEGER_CONST RBRACKET
    | ID LBRACKET REAL_CONST RBRACKET
    | var_list COMMA ID
    | var_list COMMA ID LBRACKET INTEGER_CONST RBRACKET
    | var_list COMMA ID LBRACKET REAL_CONST RBRACKET
    ;

algorithm_section:
    ALGORITHM_KW COLON /* empty */
    ;

end_main_statement:  
    END_KW MAIN_KW SEMICOLON
    ; 

%%

int yyerror() {
   printf("Called yyerror()\n");
   return  0;
}
