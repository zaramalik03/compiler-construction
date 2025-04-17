/*
 *------------------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #7: While and exit statements; mod and arrays
 * Date----------April 14, 2025
 * This is the header file that declares the functions and data structures 
 * used for constructing and handling the symbol table and the abstract syntax tree.
 *------------------------------------------------------------------------------------
 */

 #ifndef SYMBOL_TABLE_H
 #define SYMBOL_TABLE_H
 
 #define EK_OP      0
 #define EK_INT     1
 #define EK_ID      2
 #define EK_STRING  3
 #define EK_REAL    4
 #define EK_REL     5
 #define EK_BOOL    6
 #define EK_NEWLINE 7
 #define EK_PRINT   8
 #define EK_ARRAY_ACCESS 9
 
 #define OP_ADD   0
 #define OP_SUB   1
 #define OP_MUL   2
 #define OP_DIV   3
 #define OP_UMIN  4
 #define OP_MOD   5
 
 #define OP_LT    6
 #define OP_GT    7
 #define OP_AND   8
 #define OP_OR    9
 #define OP_LE    10
 #define OP_GE    11
 #define OP_NE    12
 #define OP_EQ    13
 #define OP_NOT   14
 
 
 #define ST_ASSIGN 1
 #define ST_PRINT  2
 #define ST_READ   3
 #define ST_IF     4
 #define ST_IFELSE 5
 #define ST_EXIT   6
 #define ST_WHILE  7
 
 #define DT_INTEGER 0
 #define DT_REAL 1
 
 #define VK_SCALAR 0
 #define VK_ARRAY 1

struct symbolTableEntry {
    char *name;
    char data_type;           // char is essentially an 8-bit integer
    char kind_of_variable;    // two kinds: scalar and array
    int size_of_variable;     // # of GSTAL stack entries needed for variable; size of scalar is 1, size of array is # of array entries
    int address_of_variable;  // address in GSTAL memory space
};
 
 
struct symbolTable {
    int capacity;                    // size of symbol table, dynamically grows or has a static amount
    int count;                       // increments when each variable is entered
    int next_address;                // we need to add another address space when one entry is filled
    struct symbolTableEntry *entry;  // dynamically allocated-array of symbol table entries
};

struct listnode {
    int kind_of_node;         // kind of node
    int data_type;            // real or int?
    int int_value;            // if the node has an integer data type
    float real_value;         // if the node has a real data type
    int variable_type;        // either scalar or array
    int address;              // address in GSTAL memory space
    char *name;
    // Pointers to child nodes
    struct listnode *link;  
    struct expression *exp;
    struct expression *index_expr;
    struct listnode *if_link;
    struct listnode *else_link;
    struct listnode *while_body;
    int isArray; 
};
 
struct expression {
    char kind;             // kind of variable
    char operator;
    struct expression *l_operand;
    struct expression *r_operand;
    int value;             // for ints
    int address;           // address of variable
    float real_value;      // for reals
    char *string_value;    // for strings
    char *id;              // for ids
    char data_type;        // real or int?
    int size;              // for arrays
    struct expression *index_expr;
    char *array_name; 
    struct expression *print_item; 
    struct expression *print_link;
};

extern struct listnode *list;


// *** Symbol Table Operations ***
void initialize();
void insert(char *name, char data_type, char kind_of_variable, int size_of_variable);
int lookup(char *name);
char datatype_lookup(char *name);
int generateISP();
void show();
void print(char *name);
 
#endif


