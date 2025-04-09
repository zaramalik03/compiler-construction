/*
 *--------------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #5 - Assignment Statements
 * Date----------March 24, 2025
 * This file contains the implementation of functions for managing the symbol 
 * table including initialization, insertion of symbols, lookup operations, and 
 * printing the table.
 *--------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

extern int yyparse();

struct symbolTable table;

// *** Symbol Table Operations ***

// Initializes the symbol table
void initialize(){
    table.capacity = 2000;
    table.count = 0;
    table.next_address = 0;
    table.entry = (struct symbolTableEntry *)malloc(table.capacity * sizeof(struct symbolTableEntry));

    if (!table.entry) {
        fprintf(stderr, "Error: Unable to allocate memory for symbol table.\n");
        exit(1);
    }
}

// New entry to the symbol table
void insert(char *name, char data_type, char kind_of_variable, int size_of_variable){
    struct symbolTableEntry *new_entry = &table.entry[table.count];
    new_entry->name = strdup(name);  // Allocate memory for name
    new_entry->data_type = data_type;
    new_entry->kind_of_variable = kind_of_variable;
    new_entry->size_of_variable = size_of_variable;
    new_entry->address_of_variable = table.next_address;

    table.next_address += size_of_variable;  // Reserve memory space in GSTAL
    table.count++;
}

// Looks up the address of a variable
int lookup(char *name){
    for (int i = 0; i < table.count; i++){
        if (strcmp(table.entry[i].name, name) == 0){
            return table.entry[i].address_of_variable;
        }
    }
    return -1;
}

// Looks up the data type of a variable
char datatype_lookup(char *name){
    for (int i = 0; i < table.count; i++){
        if (strcmp(table.entry[i].name, name) == 0){
            return table.entry[i].data_type;
        }
    }
    return -1;
}

// Return # of variables in the GSTAL stack
int generateISP() {
    return table.count; 
}

void show(){
    printf("\n********************** Symbol Table ***********************\n");
    printf("%-15s %-10s %-10s %-10s %-10s\n", "Name", "Type", "Kind", "Size", "Address");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < table.count; i++) {
        printf("%-15s %-10s %-10s %-10d %-10d\n",
               table.entry[i].name,
               table.entry[i].data_type == DT_INTEGER ? "INTEGER" : "REAL",
               table.entry[i].kind_of_variable == VK_SCALAR ? "SCALAR" : "ARRAY",
               table.entry[i].size_of_variable,
               table.entry[i].address_of_variable);
    }
}
