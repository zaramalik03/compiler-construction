/*
 *------------------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #4 - Constructing Symbol Table
 * Date----------March 5, 2025
 * This is the main program that contains the implementation of functions for 
 * managing the symbol table, including initialization, insertion of symbols, lookup 
 * operations, and printing the table.
 *------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

extern int yyparse();

struct symbolTable table;

// *** Symbol Table Operations ***
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


int lookup(char *name){
    for (int i = 0; i < table.count; i++){
        if (strcmp(table.entry[i].name, name) == 0){
            return table.entry[i].address_of_variable;
        }
    }
    return -1;
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


int main() {
    printf("Starting parser...\n");
    if (yyparse() == 0) {
        printf("Parsing succeeded!\n");
    } else {
        printf("Parsing failed.\n");
    }
    return 0;
}
