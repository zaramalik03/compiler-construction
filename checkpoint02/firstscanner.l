%{ 

/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #2- First Draft of Scanner
 * Date----------February 3, 2025
 * This is the first draft of a Flex scanner for each Slic token, including 
 * its regular expression and C semantic code. This prints each token name
 * with its lexeme. 
 *-------------------------------------------------------------------------
 */

#define  DEBUG 0

%}

%%

[mM][aA][iI][nN]        { if (DEBUG) printf("MAIN_KW: %s\n", yytext); }

[dD][aA][tT][aA]        { if (DEBUG) printf("DATA_KW: %s\n", yytext); }

[aA][lL][gG][oO][rR][iI][tT][hH][mM] { 
                          if (DEBUG) printf("ALGORITHM_KW: %s\n", yytext); }

[eE][nN][dD]            { if (DEBUG) printf("END_KW: %s\n", yytext); }

[iI][nN][tT][eE][gG][eE][rR] { 
                          if (DEBUG) printf("INTEGER_KW: %s\n", yytext); }   

[rR][eE][aA][lL]        { if (DEBUG) printf("REAL_KW: %s\n", yytext); }

[wW][hH][iI][lL][eE]    { if (DEBUG) printf("WHILE_KW: %s\n", yytext); }

[cC][oO][uU][nN][tT][iI][nN][gG] { 
                          if (DEBUG) printf("COUNTING_KW: %s\n", yytext); }

[uU][pP][wW][aA][rR][dD] { 
                          if (DEBUG) printf("UPWARD_KW: %s\n", yytext); }

[dD][oO][wW][nN][wW][aA][rR][dD] { 
                          if (DEBUG) printf("DOWNWARD_KW: %s\n", yytext); }

[tT][oO]                { if (DEBUG) printf("TO_KW: %s\n", yytext); }

[iI][fF]                { if (DEBUG) printf("IF_KW: %s\n", yytext); }

[eE][lL][sS][eE]        { if (DEBUG) printf("ELSE_KW: %s\n", yytext); }

[rR][eE][aA][dD]        { if (DEBUG) printf("READ_KW: %s\n", yytext); }                                                

[pP][rR][iI][nN][tT]    { if (DEBUG) printf("PRINT_KW: %s\n", yytext); }

[eE][xX][iI][tT]        { if (DEBUG) printf("EXIT_KW: %s\n", yytext); }
                
[A-Za-z][A-Za-z0-9]*    { if (DEBUG) printf("ID: %s\n", yytext); }

([0-9]+\.[0-9]+([eE][+-]?[0-9]+)?)|([0-9]+([eE][+-]?[0-9]+))  { 
                          if (DEBUG) printf("REAL_CONST: %s\n", yytext); }                               

[0-9]+                  { if (DEBUG) printf("INTEGER_CONST: %s\n", yytext); } 

\"(([^"\n]*)|(\"\"))*\" { if (DEBUG) printf("STRING_CONST: %s\n", yytext); }                                                                                      

\r?\n                   { if (DEBUG) printf("NEWLINE\n"); }

[ \t\v\f]+              { if (DEBUG) printf("WHITESPACE "); }

;                       { if (DEBUG) printf("SEMICOLON: %s\n", yytext); }

:                       { if (DEBUG) printf("COLON: %s\n", yytext); } 

#.*                     { if (DEBUG) printf("COMMENT: %s\n", yytext); }
                        
,                       { if (DEBUG) printf("COMMA: %s\n", yytext); }                                                                      

":="                    { if (DEBUG) printf("ASSIGN: %s\n", yytext); } 

"+"                     { if (DEBUG) printf("PLUS: %s\n", yytext); } 

"-"                     { if (DEBUG) printf("MINUS: %s\n", yytext); }  

"*"                     { if (DEBUG) printf("TIMES: %s\n", yytext); }                

"/"                     { if (DEBUG) printf("DIVIDE: %s\n", yytext); } 
                    
"%"                     { if (DEBUG) printf("MODULUS: %s\n", yytext); } 

"<"                     { if (DEBUG) printf("LT: %s\n", yytext); }   

"<="                    { if (DEBUG) printf("LE: %s\n", yytext); }    

">"                     { if (DEBUG) printf("GT: %s\n", yytext); }                  

">="                    { if (DEBUG) printf("GE: %s\n", yytext); }

"="                     { if (DEBUG) printf("EQ: %s\n", yytext); }

"<>"                    { if (DEBUG) printf("NE: %s\n", yytext); }

"&"                     { if (DEBUG) printf("AND: %s\n", yytext); } 

"|"                     { if (DEBUG) printf("OR: %s\n", yytext); }

"~"                     { if (DEBUG) printf("NOT: %s\n", yytext); }

"("                     { if (DEBUG) printf("LPAREN: %s\n", yytext); }

")"                     { if (DEBUG) printf("RPAREN: %s\n", yytext); }

"["                     { if (DEBUG) printf("LBRACKET: %s\n", yytext); }

"]"                     { if (DEBUG) printf("RBRACKET: %s\n", yytext); }    

"!"                     { if (DEBUG) printf("EXCLAMATION: %s\n", yytext); }   

.                       { if (DEBUG) printf("TRASH\n"); }                                         
%%

int main() {
    yylex();
    return 0;
}
