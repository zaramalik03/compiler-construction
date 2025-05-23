%{

/*
 *------------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #8: Completed Compiler
 * Date----------April 23, 2025
 * This is the bison file that is enhanced to counting statements for a Slic file 
 * and builds an abstract syntax tree and GSTAL code generator when parsing. 
 * These are all the statements needed for the completed compiler
 *------------------------------------------------------------------------------
 */

#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"


int yyerror(const char *msg);
int yylex();
%}

%union {
   char *sval;
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

%nonassoc LBRACKET RBRACKET
%nonassoc THEN
%nonassoc ELSE

%left OR
%left AND
%nonassoc EQ NE LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE MODULUS
%right NOT UMIN


%type <listpoint> statement_list statement int_var_list real_var_list 
%type <listpoint> assignment_statement print_statement read_statement if_statement if_else_statement exit_statement while_statement counting_statement
%type <exppoint> expression print_list print_item


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
    statement {
        $$ = $1;
    }
    | statement statement_list {
        $$ = malloc(sizeof(struct listnode));
        if (!$$) yyerror("Memory allocation failed");

        // Copy the statement's properties
        $$->kind_of_node = $1->kind_of_node;
        $$->data_type = $1->data_type;
        $$->int_value = $1->int_value;
        $$->real_value = $1->real_value;
        $$->variable_type = $1->variable_type;
        $$->address = $1->address;
        $$->name = $1->name;
        $$->exp = $1->exp;
        $$->index_expr = $1->index_expr;
        $$->if_link = $1->if_link;
        $$->else_link = $1->else_link;
        $$->while_body = $1->while_body;

        // Set the link
        $$->link = $2;       
    }
    ;

statement: 
    assignment_statement
    | print_statement
    | read_statement 
    | if_statement
    | if_else_statement
    | while_statement
    | counting_statement
    | exit_statement
    ;


assignment_statement:
    ID ASSIGN expression SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        if (!$$) yyerror("Memory allocation failed");
        $$->link = NULL;
        $$->exp = $3;
        $$->kind_of_node = ST_ASSIGN;
        $$->name = $1;
        $$->address = lookup($1); 
        $$->data_type = datatype_lookup($1);
        $$->variable_type = VK_SCALAR;
        $$->index_expr = NULL;
        $$->isArray = 0;
    }
    | ID LBRACKET expression RBRACKET ASSIGN expression SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->kind_of_node = ST_ASSIGN;
        $$->name = $1;
        $$->index_expr = $3;
        $$->exp = $6;
        $$->link = NULL;
        $$->address = lookup($1); 
        $$->data_type = datatype_lookup($1);
        $$->variable_type = VK_ARRAY;
        $$->isArray = 1;
    }
    ;

print_statement:  
    PRINT_KW print_list SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        if (!$$) yyerror("Memory allocation failed");
        $$->link = NULL;
        $$->exp = $2;
        $$->kind_of_node = ST_PRINT;
    }
    ;

print_list:
    print_item {
        $$ = $1;
    }
    | print_list COMMA print_item {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_PRINT;
        //$$->operator = OP_ADD;  // Use ADD as a placeholder for concatenation
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    ;

print_item:
    expression {
        $$ = $1;
    }
    | STRING_CONST {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_STRING;
        $$->string_value = $1;
    }
    | EXCLAMATION {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_NEWLINE;
    }
    ;

read_statement: 
    READ_KW ID SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        if (!$$) yyerror("Memory allocation failed");
        $$->link = NULL;
        $$->kind_of_node = ST_READ;
        $$->name = $2;

        $$->exp = malloc(sizeof(struct expression));
        if (!$$->exp) yyerror("Memory allocation failed");
        $$->exp->data_type = datatype_lookup($2);
        $$->exp->kind = EK_ID;
        $$->exp->id = $2;
        $$->exp->address = lookup($2);
    }
    ;

if_statement: 
    IF_KW expression SEMICOLON statement_list END_KW IF_KW SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind_of_node = ST_IF;
        $$->exp = $2;             // Set the condition expression
        $$->if_link = $4;         // Link to the if body statements
        $$->else_link = NULL;
    } 
    |  IF_KW expression SEMICOLON statement_list END_KW IF_KW SEMICOLON statement_list  {
        $$ = malloc(sizeof(struct listnode));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind_of_node = ST_IF;
        $$->exp = $2;             // Set the condition expression
        $$->if_link = $4;         // Link to the if body statements
        $$->link = $8;
        $$->else_link = NULL;
    } 
    ;

if_else_statement: 
    IF_KW expression SEMICOLON statement_list ELSE_KW SEMICOLON statement_list END_KW IF_KW SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        //if (!$$) yyerror("Memory allocation failed");
        $$->kind_of_node = ST_IFELSE;
        $$->exp = $2;
        $$->if_link = $4;       // If body
        $$->else_link = $7;     // Else body
        $$->link = NULL;
    }
    |  IF_KW expression SEMICOLON statement_list ELSE_KW SEMICOLON statement_list END_KW IF_KW SEMICOLON statement_list  {
        $$ = malloc(sizeof(struct listnode));
        //if (!$$) yyerror("Memory allocation failed");
        $$->kind_of_node = ST_IFELSE;
        $$->exp = $2;          
        $$->if_link = $4;        
        $$->else_link = $7; 
        $$->link = $11;
    } 
    ;

while_statement:
    WHILE_KW expression SEMICOLON statement_list END_KW WHILE_KW SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->kind_of_node = ST_WHILE;
        $$->exp = $2;
        $$->while_body = $4; 
        $$->link = NULL;
    }
    | WHILE_KW expression SEMICOLON statement_list END_KW WHILE_KW SEMICOLON statement_list {
        $$ = malloc(sizeof(struct listnode));
        $$->kind_of_node = ST_WHILE;
        $$->exp = $2;
        $$->while_body = $4; 
        $$->link = $8;
    }
    ;

counting_statement:
    COUNTING_KW ID UPWARD_KW expression TO_KW expression SEMICOLON statement_list END_KW COUNTING_KW SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->name = $2;
        $$->kind_of_node = ST_COUNTING;
        $$->count_expr1 = $4;
        $$->count_expr2 = $6;
        $$->counting_body = $8;
        $$->direction = CT_UPWARD;
        $$->link = NULL;
    }
    | COUNTING_KW ID DOWNWARD_KW expression TO_KW expression SEMICOLON statement_list END_KW COUNTING_KW SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->name = $2;
        $$->kind_of_node = ST_COUNTING;
        $$->count_expr1 = $4;
        $$->count_expr2 = $6;
        $$->counting_body = $8;
        $$->direction = CT_DOWNWARD;
        $$->link = NULL;
    }
    | COUNTING_KW ID UPWARD_KW expression TO_KW expression SEMICOLON statement_list END_KW COUNTING_KW SEMICOLON statement_list {
        $$ = malloc(sizeof(struct listnode));
        $$->name = $2;
        $$->kind_of_node = ST_COUNTING;
        $$->count_expr1 = $4;
        $$->count_expr2 = $6;
        $$->counting_body = $8;
        $$->direction = CT_UPWARD;
        $$->link = $12;
    }
    | COUNTING_KW ID DOWNWARD_KW expression TO_KW expression SEMICOLON statement_list END_KW COUNTING_KW SEMICOLON statement_list {
        $$ = malloc(sizeof(struct listnode));
        $$->name = $2;
        $$->kind_of_node = ST_COUNTING;
        $$->count_expr1 = $4;
        $$->count_expr2 = $6;
        $$->counting_body = $8;
        $$->direction = CT_DOWNWARD;
        $$->link = $12;
    }
    ;

exit_statement: 
    EXIT_KW SEMICOLON {
        $$ = malloc(sizeof(struct listnode));
        $$->kind_of_node = ST_EXIT;
        $$->link = NULL;
    }
    ;


expression:
    INTEGER_CONST {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_INT;
        $$->value = $1;
        $$->data_type = DT_INTEGER;
    }
    | REAL_CONST {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REAL;
        $$->real_value = $1;
        $$->data_type = DT_REAL;
    }
    | ID {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_ID;
        $$->id = $1;
        $$->address = lookup($1);
        $$->data_type = datatype_lookup($1);
    }
    | ID LBRACKET expression RBRACKET {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_ARRAY_ACCESS;
        $$->id = $1;
        $$->address = lookup($1);   // Base address
        $$->index_expr = $3;        // Index
        $$->data_type = datatype_lookup($1);
    }
    | expression PLUS expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_OP;
        $$->operator = OP_ADD;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression MINUS expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_OP;
        $$->operator = OP_SUB;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression TIMES expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_OP;
        $$->operator = OP_MUL;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression DIVIDE expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_OP;
        $$->operator = OP_DIV;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression MODULUS expression {
        $$ = malloc(sizeof(struct expression));
        $$->kind = EK_OP;
        $$->operator = OP_MOD;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | MINUS expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_OP;
        $$->operator = OP_UMIN;
        $$->r_operand = $2;
        $$->l_operand = NULL;
    }
    | expression AND expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_BOOL;
        $$->operator = OP_AND;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression OR expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_BOOL;
        $$->operator = OP_OR;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | NOT expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_BOOL;
        $$->operator = OP_NOT;
        $$->r_operand = $2;
        $$->l_operand = NULL;
    }
    | expression LT expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REL;
        $$->operator = OP_LT;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression GT expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REL;
        $$->operator = OP_GT;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression LE expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REL;
        $$->operator = OP_LE;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression GE expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REL;
        $$->operator = OP_GE;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression EQ expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REL;
        $$->operator = OP_EQ;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | expression NE expression {
        $$ = malloc(sizeof(struct expression));
        if (!$$) yyerror("Memory allocation failed");
        $$->kind = EK_REL;
        $$->operator = OP_NE;
        $$->l_operand = $1;
        $$->r_operand = $3;
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

end_main_statement:  
    END_KW MAIN_KW SEMICOLON 
    ;

%%
