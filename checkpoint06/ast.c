/*
 *-----------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #6- Read, print, if, and if-else statements
 * Date----------April 4, 2025
 * This is the program for generating code in GSTAL and building the abstract
 * syntax tree, which includes storing the instructions into an array and 
 * printing them out in the priority order.
 *-----------------------------------------------------------------------------
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "symbolTable.h"
 
 extern int yyparse();
 
 void codegen(struct listnode *);
 void exprgen(struct expression *, struct listnode *);
 //void if_node_gen(struct listnode *);
 void print_instructions();
 
 #define MAX_INSTRUCTIONS 1000
 char* instructions[MAX_INSTRUCTIONS];  // Array of instructions
 int integers[MAX_INSTRUCTIONS];        // Array of integers for LLI
 float reals[MAX_INSTRUCTIONS];         // Array of reals for LLF
 int addresses[MAX_INSTRUCTIONS];       // Array of addresses for LAA and ISP
 int memory[MAX_INSTRUCTIONS];   
 int instr_index = 0;  
 struct listnode *list = NULL;
 int variable_count = 0; 

int label_counter = 0;

void backpatch(int index, int target) {
   if (index >= 0 && index < instr_index) {
       integers[index] = target;
   }
}

int new_label() {
   return label_counter++;
}

 int main(){
   if (yyparse())
      printf("Syntax error\n");
   else {
      // ISP is first instruction
      if (instr_index == 0){
         instructions[instr_index] = "ISP";
         addresses[instr_index] = generateISP();
         instr_index++;
      }
      codegen(list);
      print_instructions();
   }
   return 0;
}
 
 
 /*
  * ========================================================================
  * codegen(p) -- Output a GSTAL translation of a "program" that evaluates
  * expressions based on whether it is an assignment or print statement.
  * Before evaluating for those statements, it needs to evaluate the GSTAL
  * memory space.
  * ========================================================================
  */
 
 void codegen(struct listnode *list) {
   struct listnode *current = list;
   while (current != NULL) {
      // Assignment statements: We need to load address of variable that is assigned
      if (current->kind_of_node == ST_ASSIGN){
         instructions[instr_index] = "LAA";
         addresses[instr_index] = lookup(current->name);
         instr_index++;
         // Use LOD when loading an address/variable
         // instructions[instr_index++] = "LOD\n";
 
         if (current->exp) {
            exprgen(current->exp, current);
            instructions[instr_index++] = "STO\n";
         } else {
            return;
         }
      }
      // Print statements
      else if (current->kind_of_node == ST_PRINT) {
         // If data type is real
         if (current->exp->data_type == DT_REAL){
            exprgen(current->exp, current);
            //instructions[instr_index++] = "PTF\n";
         } 
         else if (current->exp->data_type == DT_INTEGER){
            exprgen(current->exp, current);
            //instructions[instr_index++] = "PTI\n";
         } 
         // If we are printing a string
         else if (current->exp->kind == EK_STRING) {
            exprgen(current->exp, current);
         } 
         else { return; }
      }
      // If it is an if statement
//-------------------------------------------------------------------------------------
      else if (current->kind_of_node == ST_IF || current->kind_of_node == ST_IFELSE) {
         // Generate the condition expression
         exprgen(current->exp, current);

         int false_label = new_label();
         int end_label = (current->kind_of_node == ST_IFELSE) ? new_label() : -1;

         // Insert conditional jump: JPF (Jump if False)
         int jpf_index = instr_index;
         instructions[instr_index] = "JPF";
         integers[instr_index] = -1; // Placeholder to be updated later
         instr_index++;

         // Generate code for the 'if' block
         codegen(current->if_link);

         if (current->kind_of_node == ST_IFELSE) {
            // Insert unconditional jump after if-block to skip else-block
            int jmp_index = instr_index;
            instructions[instr_index] = "JMP";
            integers[instr_index] = end_label; // Placeholder
            instr_index++;
            // backpatch(jpf_index, instr_index);

            // Fix the JPF to jump to else block
            integers[jpf_index] = instr_index;

            // Generate code for the else block
            codegen(current->else_link);

            // Fix the JMP to jump after else block
            integers[jmp_index] = instr_index;
            // place_label(end_label);
            // backpatch(jmp_index, instr_index);
         } else {
            // If only an IF statement (no else), fix JPF to jump after if block
            // backpatch(jpf_index, instr_index);
            integers[jpf_index] = instr_index;
            // place_label(false_label);
         }
      }
//-------------------------------------------------------------------------------------
      // If it is a read statement
      else if (current->kind_of_node == ST_READ) {
         if (!current->name) return;

         instructions[instr_index] = "LAA";
         addresses[instr_index] = lookup(current->name);
         instr_index++;
      
         if (current->exp && current->exp->data_type == DT_INTEGER) {
             instructions[instr_index++] = "INI\n";  // Input integer
         }
         else if (current->exp && current->exp->data_type == DT_REAL) {
             instructions[instr_index++] = "INF\n";  // Input float
         }
         else {
             return;
         }
         // STO (store) at variable's memory location
         instructions[instr_index] = "STO\n";
         memory[instr_index] = lookup(current->name);  // Lookup address of variable
         instr_index++;
      }
      current = current->link;
   }
   return;
}

 
 /*
  * ========================================================================
  * exprgen(p) -- This outputs GSTAL code to evaluate integers, reals, 
  * loading addresses, and expressions for assignment and print statements
  * ========================================================================
  */
 
 // 0 for real and 1 for integer data type
 int d_type_l = 0;
 int d_type_r = 0;
 
 
 void exprgen(struct expression *p, struct listnode *l) {
   if (!p) {
     // yyerror("Invalid expression: NULL expression");
      return;
   }
   
   // Checks for kind of expressions
   if (p->kind == EK_INT) {
      instructions[instr_index] = "LLI";
      integers[instr_index] = p->value;
      instr_index++;
      // if (l && l->kind_of_node == ST_ASSIGN) {
      //    instructions[instr_index++] = "LOD\n";
      // }
   } else if (p->kind == EK_REAL) {
      instructions[instr_index] = "LLF";
      reals[instr_index] = p->real_value;
      instr_index++;
      // if (l && l->kind_of_node == ST_ASSIGN) {
      //    instructions[instr_index++] = "LOD\n";
      // }
   // If a variable is inside the statement, we need to load its address
   } else if (p->kind == EK_ID) { 
      if (p->address < 0) {
         //yyerror("Invalid variable reference: variable not found in symbol table");
         return;
      }
      instructions[instr_index] = "LAA";
      addresses[instr_index] = p->address;
      instr_index++;
      instructions[instr_index++] = "LOD\n";
   // Case when reading in a value for a variable 
   } else if (p->kind == EK_OP) { 
      // Evaluates for operands
      switch (p->operator) {
         case OP_ADD:
         case OP_SUB:
         case OP_MUL:
         case OP_DIV: {
            if (!p->l_operand || !p->r_operand) {
               //("Invalid binary operation: missing operand");
               return;
            }
            int left_type = p->l_operand->data_type;
            int right_type = p->r_operand->data_type;

            exprgen(p->l_operand, l);
            exprgen(p->r_operand, l);
          
            // Handle mixed types (convert int to float if needed)
            if ((left_type != right_type) && (l->kind_of_node != ST_PRINT)) { 
               if (left_type == DT_INTEGER) {
                  instructions[instr_index++] = "ITF\n"; 
                  instructions[instr_index++] = "STO\n";
               } else if (right_type == DT_INTEGER) {
                  instructions[instr_index++] = "FTI\n"; 
                  instructions[instr_index++] = "STO\n";
               }
               else if (left_type == DT_REAL){
                  instructions[instr_index++] = "FTI\n"; 
                  instructions[instr_index++] = "STO\n";
               } else if (right_type == DT_REAL){
                  instructions[instr_index++] = "FTI\n"; 
                  instructions[instr_index++] = "STO\n";
               }
            }
          
            // Choose the right arithmetic instruction
            char *opcode = NULL;
            switch (p->operator) {
               case OP_ADD: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "ADF\n" : "ADI\n"; break;
               case OP_SUB: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "SBF\n" : "SBI\n"; break;
               case OP_MUL: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "MLF\n" : "MLI\n"; break;
               case OP_DIV: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "DVF\n" : "DVI\n"; break;
            }
            instructions[instr_index++] = opcode;
            break;
         }
         // Separate case for UMIN since there is only one operand
         case OP_UMIN:  
            if (!p->r_operand) {
               //yyerror("Invalid unary minus: missing operand");
               return;
            }
            exprgen(p->r_operand, l);
            if (p->r_operand->data_type == DT_INTEGER) {
               instructions[instr_index++] = "NGI\n";
            } else {
               instructions[instr_index++] = "NGF\n";
            } 
            break;
      }
   } else if (p->kind == EK_REL) {
      // Evaluates for comparisons
      switch (p->operator) {
         case OP_LT:
         case OP_GT:
         case OP_LE:
         case OP_GE:
         case OP_EQ:
         case OP_NE: {
            if (!p->l_operand || !p->r_operand) {
               //yyerror("Invalid comparison: missing operand");
               return;
            }
            exprgen(p->l_operand, l);
            exprgen(p->r_operand, l);
            int left_type = p->l_operand->data_type;
            int right_type = p->r_operand->data_type;
             
            // Choose the right comparison instruction
            char *opcode = NULL;
            switch (p->operator) {
               case OP_LT: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "LTF\n" : "LTI\n"; break;
               case OP_GT: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "GTF\n" : "GTI\n"; break;
               case OP_LE: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "LEF\n" : "LEI\n"; break;
               case OP_GE: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "GEF\n" : "GEI\n"; break;
               case OP_EQ: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "EQF\n" : "EQI\n"; break;
               case OP_NE: opcode = (left_type == DT_REAL || right_type == DT_REAL) ? "NEF\n" : "NEI\n"; break;
            }
            instructions[instr_index++] = opcode;
            break;
         }
      }
   } else if (p->kind == EK_BOOL) {
      // Evaluates for booleans
      switch (p->operator) {
         case OP_AND: 
         case OP_OR: {
            if (!p->l_operand || !p->r_operand) {
               //yyerror("Invalid boolean operation: missing operand");
               return;
            }
            exprgen(p->l_operand, l);
            exprgen(p->r_operand, l);
            int left_type = p->l_operand->data_type;
            int right_type = p->r_operand->data_type;
            if (left_type == DT_REAL || right_type == DT_REAL) {
               // Treating & as multiplication for boolean results 0.0 or 1.0
               instructions[instr_index++] = "MLF\n"; 
            } else {
               instructions[instr_index++] = "MLI\n";
            }
            break;
         }
         case OP_NOT:   
            if (!p->r_operand) {
               //yyerror("Invalid NOT operation: missing operand");
               return;
            }
            exprgen(p->r_operand, l);
            if (p->r_operand->data_type == DT_REAL) {
               instructions[instr_index++] = "NTF\n";  // NOT for float
            } else {
               instructions[instr_index++] = "NTI\n";  // NOT for integer
            }
            break;
      }
   } 
   else if (p->kind == EK_PRINT){
      if (!p->l_operand || !p->r_operand) {
         return;
      }
      exprgen(p->l_operand, l);
      exprgen(p->r_operand, l);
      if (p->r_operand->data_type == DT_INTEGER){
         if (instructions[instr_index-1] != "PTL\n") {
            instructions[instr_index++] = "PTI\n";
         }
      }
      if (p->r_operand->data_type == DT_REAL){
         if (instructions[instr_index-1] != "PTL\n") {
            instructions[instr_index++] = "PTF\n";
         }
      }
    
   }
   else if (p->kind == EK_STRING) {
      // String literals are handled in the print statement
      struct expression *printItem = p;
      char *str = printItem->string_value;
      for (int i = 0; str[i] != '\0'; i++){
         if ((i != 0) && (i != strlen(str) - 1)) {
            instructions[instr_index] = "LLI";
            integers[instr_index] = str[i];
            instr_index++;
            instructions[instr_index++] = "PTC\n";
         }
      }
   } else if (p->kind == EK_NEWLINE) {
      instructions[instr_index++] = "PTL\n";
   }
   return;
}

//Prints all the GSTAl instructions
void print_instructions() {
   for (int i = 0; i < instr_index; i++) {
      if (strcmp(instructions[i], "LLF") == 0) {
         printf("%s %f\n", instructions[i], reals[i]);
      } else if (strcmp(instructions[i], "LLI") == 0 || strcmp(instructions[i], "JPF") == 0 || strcmp(instructions[i], "JMP") == 0) {
         printf("%s %d\n", instructions[i], integers[i]);
      } else if ((strcmp(instructions[i], "LAA") == 0) || (strcmp(instructions[i], "ISP") == 0)) {
         printf("%s %d\n", instructions[i], addresses[i]);
      } else {
         printf("%s", instructions[i]);
      }
   }
   printf("%s", "HLT\n");
}

