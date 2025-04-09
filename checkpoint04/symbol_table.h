/*
 *--------------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #4 - Constructing Symbol Table
 * Date----------March 5, 2025
 * This is the header file that declares the functions and data structures 
 * used for handling the symbol table, providing an interface for initialization, 
 * insertion, lookup, and printing.
 *--------------------------------------------------------------------------------
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

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


// *** Symbol Table Operations ***
void initialize();
void insert(char *name, char data_type, char kind_of_variable, int size_of_variable);
int lookup(char *name);
void show();

#endif
