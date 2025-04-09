/*
 *-------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #3- Parsing Data Section
 * Date----------February 14, 2025
 * This is the main program written in C for the application that notifies
 * if parsing succeeds or fails
 *-------------------------------------------------------------------------
 */

#include "y.tab.h"
#include <stdio.h>

int main()
{
   int parse_result;

   printf("\n");
   parse_result = yyparse();

   if (parse_result == 0) {
      printf("Parsing succeeded.\n");
   }
   else {
      printf("Parsing failed.\n");
   }

   printf("\n");
   return 0;
}
