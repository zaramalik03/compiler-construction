%{

/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #5 - Assignment Statements
 * Date----------March 24, 2025
 * This is the bison file that is enhanced to parse assignment and minimal
 * print statements for a Slic file and builds an abstract syntax tree
 * and GSTAL code generator when parsing. 
 *-------------------------------------------------------------------------
 */

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"


int yyerror(const char *msg);
int yylex();
extern struct listnode *list;
%}

%union {
   char *sval;
   char *id;
   int ival;
   double rval;
   struct listnode *listpoint;
   struct expression *exppoint;
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

%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%nonassoc UMIN


%type <listpoint> statement_list int_var_list real_var_list
%type <exppoint> expression print_list print_item

%debug

%%

program: 
    main_statement data_section algorithm_section end_main_statement
    ;

main_statement:      
    MAIN_KW SEMICOLON {
        initialize();
    }
    ;

data_section:        
    DATA_KW COLON declarations
    | DATA_KW COLON 
    ;

declarations:
    declaration
    | declarations declaration 
    ;    

declaration:
    INTEGER_KW COLON int_var_list SEMICOLON
    | REAL_KW COLON real_var_list SEMICOLON
    ;

int_var_list:
    ID {
        if (!lookup($1)) {
            yyerror($1);
        } else {
            insert($1, DT_INTEGER, VK_SCALAR, 1);
        }
    }
    | ID LBRACKET INTEGER_CONST RBRACKET {
        if (!lookup($1)) {
            yyerror($1);
        } else {
            insert($1, DT_INTEGER, VK_ARRAY, $3);
        }
    }
    | int_var_list COMMA ID {
        if (!lookup($3)) {
            yyerror($3);
        } else {
            insert($3, DT_INTEGER, VK_SCALAR, 1);
        }
    }
    | int_var_list COMMA ID LBRACKET INTEGER_CONST RBRACKET {
        if (!lookup($3)) {
            yyerror($3);
        } else {
            insert($3, DT_INTEGER, VK_ARRAY, $5);
        }
    }
    ;

real_var_list:
    ID {
        if (!lookup($1)) {
            yyerror($1);
        } else {
            insert($1, DT_REAL, VK_SCALAR, 1);
        }
    }
    | ID LBRACKET INTEGER_CONST RBRACKET {
        if (!lookup($1)) {
            yyerror($1);
        } else {
            insert($1, DT_REAL, VK_ARRAY, $3);
        }
    }
    | real_var_list COMMA ID {
        if (!lookup($3)) {
            yyerror($3);
        } else {
            insert($3, DT_REAL, VK_SCALAR, 1);
        }
    }
    | real_var_list COMMA ID LBRACKET INTEGER_CONST RBRACKET {
        if (!lookup($3)) {
            yyerror($3);
        } else {
            insert($3, DT_REAL, VK_ARRAY, $5);
        }
    }
    ;


algorithm_section:
    ALGORITHM_KW COLON statement_list { list = $3; }
    | ALGORITHM_KW COLON
    ;

statement_list:
    ID ASSIGN expression SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->link = NULL;
        $$->exp = $3;
        $$->kind_of_node = ST_ASSIGN;
        $$->name = $1;
    }
    | ID ASSIGN expression SEMICOLON statement_list {
        $$ = malloc(sizeof(struct listnode));
        $$->link = $5;
        $$->exp = $3;
        $$->kind_of_node = ST_ASSIGN;
        $$->name = $1;
    }
    | PRINT_KW print_list SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->link = NULL;
        $$->exp = $2;
        $$->kind_of_node = ST_PRINT;
    }
    | PRINT_KW print_list SEMICOLON statement_list {
        $$ = malloc(sizeof(struct listnode));
        $$->link = $4;
        $$->exp = $2;
        $$->kind_of_node = ST_PRINT;
    } 
    ;

print_list:
    print_item
    | print_list COMMA print_item
    ;

print_item:
    expression 
    | STRING_CONST {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_STRING;
        $$->string_value = $1; 
    }
    | EXCLAMATION {
        $$ = 0;
    }
    ;

expression:
    INTEGER_CONST {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_INT;
        $$->value = $1;
        $$->data_type = DT_INTEGER;
    }
    | REAL_CONST {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REAL;
        $$->real_value = $1;
        $$->data_type = DT_REAL;
    }
    | ID {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_ID;
        $$->id = $1;
        $$->address = lookup($1);
        $$->data_type = datatype_lookup($1); 
    }
    | expression PLUS expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_OP;
        $$->operator = OP_ADD;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression MINUS expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_OP;
        $$->operator = OP_SUB;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression TIMES expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_OP;
        $$->operator = OP_MUL;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression DIVIDE expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_OP;
        $$->operator = OP_DIV;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | MINUS expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_OP;
        $$->operator = OP_UMIN;
        $$->r_operand = $2;
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    | expression LT expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REL;
        $$->operator = OP_LT;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression GT expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REL;
        $$->operator = OP_GT;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression LE expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REL;
        $$->operator = OP_LE;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression GE expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REL;
        $$->operator = OP_GE;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression EQ expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REL;
        $$->operator = OP_EQ;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression NE expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_REL;
        $$->operator = OP_NE;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression AND expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_BOOL;
        $$->operator = OP_AND;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression OR expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_BOOL;
        $$->operator = OP_OR;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    ;

end_main_statement:  
    END_KW MAIN_KW SEMICOLON 
    ;

%%

int yyerror(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    return 1;
}
