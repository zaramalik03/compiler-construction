/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #5- Assignment Statements
 * Date----------March 24, 2025
 * This is the main program written in C for the application that notifies
 * if parsing succeeds or fails
 *-------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

extern int yyparse();
void codegen();

int main()
{
   int parse_result;
   parse_result = yyparse();
   if (parse_result == 0) {
      codegen();
   }
   else {
      printf("Syntax error\n"); 
      return(0);
   }
}
