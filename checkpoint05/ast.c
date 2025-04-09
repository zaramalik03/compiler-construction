/*
 *-----------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #5- Assignment Statements
 * Date----------March 24, 2025
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
 
 void codegen();
 void exprgen(struct expression *, struct listnode *);
 void print_instructions();
 
 #define MAX_INSTRUCTIONS 1000
 char* instructions[MAX_INSTRUCTIONS];  // Array of instructions
 int integers[MAX_INSTRUCTIONS];        // Array of integers for LLI
 float reals[MAX_INSTRUCTIONS];         // Array of reals for LLF
 int addresses[MAX_INSTRUCTIONS];       // Array of addresses for LAA and ISP
 int instr_index = 0;   
 
 
 /*
  * ========================================================================
  * codegen(p) -- Output a GSTAL translation of a "program" that evaluates
  * expressions based on whether it is an assignment or print statement.
  * Before evaluating for those statements, it needs to evaluate the GSTAL
  * memory space.
  * ========================================================================
  */
 struct listnode *list = NULL;
 
 void codegen() {
    struct listnode *current = list;
    while (current != NULL) {
      // ISP is first instruction
       if (instr_index == 0){
          instructions[instr_index] = "ISP";
          addresses[instr_index] = generateISP();
          instr_index++;
       }
       // We need to load address of variable that is assigned
       if (current->kind_of_node == ST_ASSIGN){
          instructions[instr_index] = "LAA";
          addresses[instr_index] = lookup(current->name);
          instr_index++;
 
          exprgen(current->exp, current);
          instructions[instr_index] = "STO\n";
          instr_index++;
       } else {
         // If is isn't an assign statement, it is a print statement
          exprgen(current->exp, current);
          if (current->exp->data_type == DT_REAL){
             instructions[instr_index] = "PTF\n";
             instr_index++;
          } if (current->exp->data_type == DT_INTEGER) {
             instructions[instr_index] = "PTI\n";
             instr_index++;
          }
       }
       current = current->link;
    }
    // Prints instructions after codegen and exprgen are done
    print_instructions();
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
   // Checks for kind of expressions
    if (p->kind == EK_INT) {
          instructions[instr_index] = "LLI";
          integers[instr_index] = p->value;
          instr_index++;
    } 
    else if (p->kind == EK_REAL){
          instructions[instr_index] = "LLF";
          reals[instr_index] = p->real_value;
          instr_index++;
     }
     // If a variable is inside the statement, we need to load its address
     else if (p->kind == EK_ID){
       instructions[instr_index] = "LAA";
       addresses[instr_index] = p->address;
       instr_index++;
       if (l->kind_of_node == ST_ASSIGN){
          instructions[instr_index] = "LOD\n";
          instr_index++;
       } 
     } else if (p->kind == EK_OP){
         // Evaluates for operands
          switch (p->operator) {
             case OP_ADD:
             case OP_SUB:
             case OP_MUL:
             case OP_DIV: {
                 exprgen(p->l_operand, l);
                 exprgen(p->r_operand, l);
                 int left_type = p->l_operand->data_type;
                 int right_type = p->r_operand->data_type;
             
                 // Handle mixed types (convert int to float if needed)
                 if (left_type != right_type) {
                     if (left_type == DT_INTEGER) {
                         instructions[instr_index++] = "ITF\n"; // Convert left int to float
                     } else {
                         instructions[instr_index++] = "ITF\n"; // Convert right int to float
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
             case OP_UMIN:  exprgen(p->r_operand, l);
                            if (p->r_operand->data_type == DT_INTEGER){
                               instructions[instr_index] = "NGI\n";
                            }
                            else {
                               instructions[instr_index] = "NGF\n";
                            } 
                            instr_index++;
                            break;
          }
       } else if (p->kind == EK_REL){
         // Evaluates for comparisons
          switch (p->operator) {
             case OP_LT:
             case OP_GT:
             case OP_LE:
             case OP_GE:
             case OP_EQ:
             case OP_NE: {
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
       } else if (p->kind == EK_BOOL){
         //Evaluates for booleans
          switch (p->operator) {
             case OP_AND: 
             case OP_OR: {
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
          }
       }
       return;
    }
 
//Prints all the GSTAl instructions
 void print_instructions() {
    for (int i = 0; i < instr_index; i++){
       if (strcmp(instructions[i], "LLF") == 0){
          printf("%s %f\n",instructions[i], reals[i]);
       } else if (strcmp(instructions[i], "LLI") == 0) {
          printf("%s %d\n",instructions[i], integers[i]);
       } else if ((strcmp(instructions[i],"LAA") == 0) || (strcmp(instructions[i],"ISP") == 0)) {
         printf("%s %d\n",instructions[i], addresses[i]);
       } else {
          printf("%s", instructions[i]);
       }
    }
    printf("%s", "HLT\n");
 }
