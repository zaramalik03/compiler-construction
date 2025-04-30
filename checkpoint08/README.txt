/*
 *------------------------------------------------------------------------------
 * Name----------Zara Malik
 * Course--------CS4223-01
 * Assignment----Checkpoint #8: Completed Compiler
 * Date----------April 23, 2025
 * This is README file that talks about the compiler as a whole along with any
 * bugs, issues, or features it has.
 *------------------------------------------------------------------------------
 */

This here is my completed compiler that includes a constructed symbol table with the functions 
to initialize, insert, and lookup variables from a Slic program. The program include a parser in
Bison and a scanner in Flex, and the parser is enhanced to support the following:
    * assignment statements for variables, numbers, and array indexes
    * print statements
    * read statements
    * if/else statements
    * while statements
    * counting statements
    * exit statements
    * AND, OR, NOT operators
    * comparison operators (>, <. <>, etc)
    * mathematical operations (+,-,/, etc)
Each of them are created as list nodes. The values, variable names inside the statements and the 
statement bodies used are in the expression nodes, which are linked to the list nodes. This program has been
tested with numerous files throughout the course, and each of them do their part well. However, there have 
been a few issues regarding a couple of Slic files such as incorrect numbering regarding printing the array
indexes and the counting index itself on one file, but everything beign correct on other files. In fact,
there is a single execution error with an out-of-bounds JPF value, which only happens when I add other
if statements. Otherwise, the JMP and JPF values are correct on other loops and statements for many other 
programs. Initially, I had hundreds of reduce/shift conflicts, but I have reduced most of them, aligning 
all the operations correctly. 

I tried my best to make this compiler correct as possible as there are a few issues still. 

