%{

/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #6- Read, print, if, and if-else statements
 * Date----------April 4, 2025
 * This is the flex scanner file that includes all necessary tokens and its 
 * lexemes in order to parse the program successfully. 
 *-------------------------------------------------------------------------
 */

#define  DEBUG 0
#include "y.tab.h"
#include <stdio.h>
#include <string.h>

extern int yylineno;
extern char *yytext;

%}

%option yylineno

%%

[mM][aA][iI][nN]        { if (DEBUG) printf("MAIN_KW: %s\n", yytext); return(MAIN_KW); }

[dD][aA][tT][aA]        { if (DEBUG) printf("DATA_KW: %s\n", yytext); return(DATA_KW); }

[aA][lL][gG][oO][rR][iI][tT][hH][mM] { 
                          if (DEBUG) printf("ALGORITHM_KW: %s\n", yytext); return(ALGORITHM_KW); }

[eE][nN][dD]            { if (DEBUG) printf("END_KW: %s\n", yytext); return(END_KW); }

[iI][nN][tT][eE][gG][eE][rR] { 
                          if (DEBUG) printf("INTEGER_KW: %s\n", yytext); return(INTEGER_KW); }   

[rR][eE][aA][lL]        { if (DEBUG) printf("REAL_KW: %s\n", yytext); return(REAL_KW); }

[wW][hH][iI][lL][eE]    { if (DEBUG) printf("WHILE_KW: %s\n", yytext); return(WHILE_KW); }

[cC][oO][uU][nN][tT][iI][nN][gG] { 
                          if (DEBUG) printf("COUNTING_KW: %s\n", yytext); return(COUNTING_KW); }

[uU][pP][wW][aA][rR][dD] { 
                          if (DEBUG) printf("UPWARD_KW: %s\n", yytext); return(UPWARD_KW); }

[dD][oO][wW][nN][wW][aA][rR][dD] { 
                          if (DEBUG) printf("DOWNWARD_KW: %s\n", yytext); return(DOWNWARD_KW); }

[tT][oO]                { if (DEBUG) printf("TO_KW: %s\n", yytext); return(TO_KW); }

[iI][fF]                { if (DEBUG) printf("IF_KW: %s\n", yytext); return(IF_KW); }

[eE][lL][sS][eE]        { if (DEBUG) printf("ELSE_KW: %s\n", yytext); return(ELSE_KW); }

[rR][eE][aA][dD]        { if (DEBUG) printf("READ_KW: %s\n", yytext); return(READ_KW); }                                                

[pP][rR][iI][nN][tT]    { if (DEBUG) printf("PRINT_KW: %s\n", yytext); return(PRINT_KW); }

[eE][xX][iI][tT]        { if (DEBUG) printf("EXIT_KW: %s\n", yytext); return(EXIT_KW); }
                
[A-Za-z_][A-Za-z0-9_]*    { if (DEBUG) printf("ID: %s\n", yytext); 
                          yylval.sval = strdup(yytext);
                          return(ID); }

([0-9]+\.[0-9]+([eE][+-]?[0-9]+)?)|([0-9]+([eE][+-]?[0-9]+))  { 
                          if (DEBUG) printf("REAL_CONST: %s\n", yytext); 
                          yylval.rval = atof(yytext);
                          return(REAL_CONST); }                               

[0-9]+                  { if (DEBUG) printf("INTEGER_CONST: %s\n", yytext); 
                          yylval.ival = atoi(yytext);
                          return(INTEGER_CONST); }    

\"(([^"\n]*)|(\"\"))*\" { if (DEBUG) printf("STRING_CONST: %s\n", yytext); 
                          yylval.sval = strdup(yytext);
                          return(STRING_CONST); }                                                                                                               

\r?\n                   { if (DEBUG) printf("NEWLINE\n"); }

[ \t\v\f]+              { /* whitespace */ }

;                       { if (DEBUG) printf("SEMICOLON: %s\n", yytext); return(SEMICOLON); }

:                       { if (DEBUG) printf("COLON: %s\n", yytext); return(COLON); } 

#.*                     { if (DEBUG) printf("COMMENT: %s\n", yytext); }
                        
,                       { if (DEBUG) printf("COMMA: %s\n", yytext); return(COMMA); }      

":="                    { if (DEBUG) printf("ASSIGN: %s\n", yytext); return(ASSIGN); } 

"+"                     { if (DEBUG) printf("PLUS: %s\n", yytext); return(PLUS); } 

"-"                     { if (DEBUG) printf("MINUS: %s\n", yytext); return(MINUS); }  

"*"                     { if (DEBUG) printf("TIMES: %s\n", yytext); return(TIMES); }                

"/"                     { if (DEBUG) printf("DIVIDE: %s\n", yytext); return(DIVIDE); } 
                    
"%"                     { if (DEBUG) printf("MODULUS: %s\n", yytext); return(MODULUS); } 

"<"                     { if (DEBUG) printf("LT: %s\n", yytext); return(LT); }   

"<="                    { if (DEBUG) printf("LE: %s\n", yytext); return(LE); }    

">"                     { if (DEBUG) printf("GT: %s\n", yytext); return(GT); }                  

">="                    { if (DEBUG) printf("GE: %s\n", yytext); return(GE); }

"="                     { if (DEBUG) printf("EQ: %s\n", yytext); return(EQ); }

"<>"                    { if (DEBUG) printf("NE: %s\n", yytext); return(NE); }

"&"                     { if (DEBUG) printf("AND: %s\n", yytext); return(AND); } 

"|"                     { if (DEBUG) printf("OR: %s\n", yytext); return(OR); }

"~"                     { if (DEBUG) printf("NOT: %s\n", yytext); return(NOT); }

"("                     { if (DEBUG) printf("LPAREN: %s\n", yytext); return(LPAREN); }

")"                     { if (DEBUG) printf("RPAREN: %s\n", yytext); return(RPAREN); }

"["                     { if (DEBUG) printf("LBRACKET: %s\n", yytext); return(LBRACKET); }

"]"                     { if (DEBUG) printf("RBRACKET: %s\n", yytext); return(RBRACKET); }   

"!"                     { if (DEBUG) printf("EXCLAMATION: %s\n", yytext); return(EXCLAMATION); }  

.                       { if (DEBUG) printf("TRASH\n"); return(TRASH); }                                         
%%


int yyerror(const char *s) {
    fprintf(stderr, "Syntax Error: %s at line %d\n", s, yylineno);
    return 0;
}
