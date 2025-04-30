/*
 *-----------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #8: Completed Compiler
 * Date----------April 23, 2025
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
 int array_or_index[MAX_INSTRUCTIONS];

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
       if (current->kind_of_node == ST_EXIT){
          instructions[instr_index++] = "HLT";
          break;
       }
       // Assignment statements: We need to load address of variable that is assigned
       else if (current->kind_of_node == ST_ASSIGN && current->index_expr != NULL) {
          // Evaluate the array index expression (push index on stack)
          exprgen(current->index_expr, current);
          instructions[instr_index] = "LAA";
          addresses[instr_index] = lookup(current->name);
          instr_index++;
 
          instructions[instr_index++] = "ADI\n";   // Index + base
          exprgen(current->exp, current);
          instructions[instr_index++] = "STO\n";
       } else if (current->kind_of_node == ST_ASSIGN){
          instructions[instr_index] = "LAA";
          addresses[instr_index] = lookup(current->name);
          instr_index++;
          exprgen(current->exp, current);
          instructions[instr_index++] = "STO\n";
       }
       // Print statements
       else if (current->kind_of_node == ST_PRINT) {
          // If data type is real
          if (current->exp) {
             if (current->exp->data_type == DT_REAL){
                exprgen(current->exp, current);
                if (instructions[instr_index-1] != "PTL\n" && instructions[instr_index-1] != "PTC\n" && instructions[instr_index-1] != "PTF\n" && instructions[instr_index-1] != "PTI\n") {
                  instructions[instr_index++] = "PTF\n";
                }
             } 
             else if (current->exp->data_type == DT_INTEGER){
                exprgen(current->exp, current);
                if (instructions[instr_index-1] != "PTL\n" && instructions[instr_index-1] != "PTC\n" && instructions[instr_index-1] != "PTI\n" && instructions[instr_index-1] != "PTF\n") {
                  instructions[instr_index++] = "PTI\n";
                }
             } 
             // If we are printing a string
             else if (current->exp->kind == EK_STRING) {
                exprgen(current->exp, current);
             } 
          }
       }
       // If it is an if statement
       else if (current->kind_of_node == ST_IF || current->kind_of_node == ST_IFELSE) {
         int loop_start = instr_index;
         exprgen(current->exp, current);
 
          // Insert conditional jump: JPF (Jump if False)
          int jpf_index = instr_index;
          instructions[instr_index] = "JPF";
          integers[instr_index] = -1;  // Temporary placeholder
          instr_index++;
 
          // Generate code for the 'if' block
          codegen(current->if_link);
 
          if (current->kind_of_node == ST_IFELSE) {
             // Insert unconditional jump after if-block to skip else-block
             int jmp_index = instr_index;
             instructions[instr_index] = "JMP";
             integers[instr_index] = loop_start;
             instr_index++;
             // Fix the JPF to jump to else block
             integers[jpf_index] = instr_index;
 
             // Generate code for the else block
             codegen(current->else_link);
 
             // Fix the JMP to jump after else block
             integers[jmp_index] = instr_index;
          } else {
             // If only an if statement, fix JPF to jump after if block
             integers[jpf_index] = instr_index;
          }
       }
      // If it is a while statement
      else if (current->kind_of_node == ST_WHILE) {
         int loop_start = instr_index;
         exprgen(current->exp, current);
         
         // JPF to exit (backpatch later)
         int jpf_index = instr_index;
         instructions[instr_index] = "JPF";
         integers[instr_index] = -1;  // Temporary placeholder
         instr_index++;

         // Generate loop body
         codegen(current->while_body);
         
         // Jump back to start
         instructions[instr_index] = "JMP";
         integers[instr_index] = loop_start;
         instr_index++;

         integers[jpf_index] = instr_index;
     }
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
       } else if (current->kind_of_node == ST_COUNTING) {
         // Initialize counter
         int counter_addr = lookup(current->name);

         instructions[instr_index] = "LAA";
         addresses[instr_index] = counter_addr;
         instr_index++;
         exprgen(current->count_expr1, current);  // Initial value
         instructions[instr_index++] = "STO\n";
         
         // Loop start (address stored for later jump)
         int loop_start = instr_index;
         
         // Check condition (counter < limit for upward, counter > limit for downward)
         instructions[instr_index] = "LAA";
         addresses[instr_index] = counter_addr;
         instr_index++;
         instructions[instr_index++] = "LOD\n";

         exprgen(current->count_expr2, current);  // Limit value
         if (current->direction == CT_UPWARD) {
            instructions[instr_index++] = "LTI\n";  // Check counter < limit
         } else {
            instructions[instr_index++] = "GTI\n";  // Check counter > limit
         }
         
         // JPF to exit (backpatch later)
         int jpf_index = instr_index;
         instructions[instr_index] = "JPF";
         integers[instr_index] = -1;  // Temporary placeholder
         instr_index++;
         
         // Generate loop body
         codegen(current->counting_body);
         
         // Increment/decrement counter
         instructions[instr_index] = "LAA";
         addresses[instr_index] = counter_addr;
         instr_index++;
         instructions[instr_index] = "LAA";
         addresses[instr_index] = counter_addr;
         instr_index++;
         instructions[instr_index++] = "LOD\n";
         instructions[instr_index] = "LLI";
         integers[instr_index] = 1;
         instr_index++;
         if (current->direction == CT_UPWARD) {
            instructions[instr_index++] = "ADI\n";  // counter += 1
         } else {
            instructions[instr_index++] = "SBI\n";  // counter -= 1
         }
         instructions[instr_index++] = "STO\n";
         
         // Jump back to loop start
         instructions[instr_index] = "JMP";
         integers[instr_index] = loop_start;
         instr_index++;
         
         // Backpatch JPF to here
         integers[jpf_index] = instr_index;
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
       perror("Invalid expression: NULL expression");
       return;
    }
    // Checks for kind of expressions
    if (p->kind == EK_INT) {
       instructions[instr_index] = "LLI";
       integers[instr_index] = p->value;
       instr_index++;
    } else if (p->kind == EK_REAL) {
       instructions[instr_index] = "LLF";
       reals[instr_index] = p->real_value;
       instr_index++;
    // If a variable is inside the statement, we need to load its address
    } else if (p->kind == EK_ID || p->kind == EK_ARRAY_ACCESS) {
      if (p->address < 0) {
          //yyerror("Invalid variable reference: variable not found in symbol table");
          return;
      }
      if (p->kind == EK_ARRAY_ACCESS){
         exprgen(p->index_expr, l);               // Get array index
         instructions[instr_index] = "LAA";
         addresses[instr_index] = lookup(p->id);  // Get array base address
         instr_index++; 
         instructions[instr_index] = "ADI\n";     // Base + index
         instr_index++;
         instructions[instr_index] = "LOD\n";     // Load the value
         instr_index++;
         if (l->kind_of_node == ST_PRINT) {
            if (instructions[instr_index-1] != "PTL\n" && instructions[instr_index-1] != "PTC\n" && instructions[instr_index-1] != "PTI\n" && instructions[instr_index-1] != "PTF\n") {
               if (datatype_lookup(p->id) == DT_REAL) {
                  instructions[instr_index++] = "PTF\n";
               } else {
                  instructions[instr_index++] = "PTI\n";
               }
            }
         }
      } else {
         instructions[instr_index] = "LAA";
         addresses[instr_index] = lookup(p->id);
         instr_index++;
         instructions[instr_index] = "LOD\n";
         instr_index++;
         if (l->kind_of_node == ST_PRINT) {
            if (instructions[instr_index-1] != "PTL\n" && instructions[instr_index-1] != "PTC\n" && instructions[instr_index-1] != "PTI\n" && instructions[instr_index-1] != "PTF\n") {
               if (datatype_lookup(p->id) == DT_REAL) {
                  instructions[instr_index++] = "PTF\n";
               } else {
                  instructions[instr_index++] = "PTI\n";
               }
            }
         }
      }
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
          case OP_MOD: {
             if (!p->l_operand || !p->r_operand) {
                return;
             }
             int left_type = p->l_operand->data_type;
             int right_type = p->r_operand->data_type;
 
             exprgen(p->l_operand, l);
             exprgen(p->r_operand, l);
 
 
             // Divide a/b
             if (left_type == DT_REAL || right_type == DT_REAL) {
                instructions[instr_index++] = "DVF\n";
             } else {
                instructions[instr_index++] = "DVI\n";
             };
 
             exprgen(p->r_operand, l);
             // Multiply by b
             if (left_type == DT_REAL || right_type == DT_REAL) {
                instructions[instr_index++] = "MLF\n";
             } else {
                instructions[instr_index++] = "MLI\n";
             }
 
             exprgen(p->l_operand, l);
             // Subtract from original a
             if (left_type == DT_REAL || right_type == DT_REAL) {
                instructions[instr_index++] = "SBF\n";
             } else {
                instructions[instr_index++] = "SBI\n";
             }
 
             if (left_type == DT_REAL || right_type == DT_REAL) {
                instructions[instr_index++] = "NGF\n";
             } else {
                instructions[instr_index++] = "NGI\n";
             }
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
             if (left_type == DT_REAL || right_type == DT_REAL){
               instructions[instr_index++] = "ITF\n";
             }
              
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
         case OP_AND: {
            exprgen(p->l_operand, l);
            exprgen(p->r_operand, l);
            if (p->l_operand->data_type == DT_REAL || p->r_operand->data_type == DT_REAL) {
                instructions[instr_index++] = "MLF\n";
                instructions[instr_index] = "LLF";
                reals[instr_index] = 0.0;
                instr_index++;
                instructions[instr_index++] = "GTF\n";
            } else {
                instructions[instr_index++] = "MLI\n";
                instructions[instr_index] = "LLI";
                integers[instr_index] = 0;
                instr_index++;
                instructions[instr_index++] = "GTI\n";
            }
            break;
        }
        
        case OP_OR: {
            exprgen(p->l_operand, l);
            exprgen(p->r_operand, l);
            if (p->l_operand->data_type == DT_REAL || p->r_operand->data_type == DT_REAL) {
                instructions[instr_index++] = "ADF\n";
                instructions[instr_index] = "LLF";
                reals[instr_index] = 0.0;
                instr_index++;
                instructions[instr_index++] = "GTF\n";
            } else {
                instructions[instr_index++] = "ADI\n";
                instructions[instr_index] = "LLI";
                integers[instr_index] = 0;
                instr_index++;
                instructions[instr_index++] = "GTI\n";
            }
            break;
        }
          case OP_NOT:  
             if (!p->r_operand) {
                //yyerror("Invalid NOT operation: missing operand");
                return;
             }
            exprgen(p->r_operand, l);
            instructions[instr_index] = "LLI";  
            integers[instr_index] = 0;
            instr_index++;
            instructions[instr_index++] = "LEI";  
             break;
       }
    } 
    else if (p->kind == EK_PRINT){
       if (!p->l_operand || !p->r_operand) {
          return;
       }
       exprgen(p->l_operand, l);
       exprgen(p->r_operand, l);
     
    }
    else if (p->kind == EK_STRING) {
       // String literals are handled in the print statement
       // struct expression *printItem = p;
       char *str = p->string_value;
       for (int i = 0; str[i] != '\0'; i++){
          if ((i != 0) && (i != strlen(str)-1)) {
             instructions[instr_index] = "LLI";
             integers[instr_index] = str[i];
             instr_index++;
             instructions[instr_index++] = "PTC\n";
          }
          if (i == strlen(str)-1) { break; }
       }
    } else if (p->kind == EK_NEWLINE) {
       instructions[instr_index++] = "PTL\n";
    }
    return;
 }
 
 //Prints all the GSTAl instructions
 void print_instructions() {
    for (int i = 0; i < instr_index; i++) {
       if (strcmp(instructions[i], "HLT") == 0) {
          break;
       }
       else if (strcmp(instructions[i], "LLF") == 0) {
          printf("%s %f\n", instructions[i], reals[i]);
       } else if (strcmp(instructions[i], "LLI") == 0 || strcmp(instructions[i], "JPF") == 0 || strcmp(instructions[i], "JMP") == 0 || strcmp(instructions[i], "LBL") == 0) {
          printf("%s %d\n", instructions[i], integers[i]);
       } else if ((strcmp(instructions[i], "LAA") == 0) || (strcmp(instructions[i], "ISP") == 0)) {
          printf("%s %d\n", instructions[i], addresses[i]);
       } else {
          printf("%s", instructions[i]);
       }
    } 
    if (instructions[instr_index] != "HLT\n"){
      printf("%s", "HLT\n");
    }
 }
 
 
