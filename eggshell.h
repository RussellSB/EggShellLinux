#pragma once

/* imports used for various function calls */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <tgmath.h>
#include <sys/wait.h>
#include "linenoise.h"
#include <ctype.h>


/* --in vrblController.c-- */

/* Constants defined */
#define MAX_CHAR 300 //maximum number of characters in a string, used for cwd

//Struct Declarations
typedef struct shellVariable Var; //instance of a shell variable
typedef struct variableArray VarArr; //stores multiple shell variables

//Method Declarations
int validateVarName(char* name); //check inputted varName for validity
void addVar(char* name, char* content); //adds variable to initialised variable storage
char * getVarValue(const char * variableName); //gets the variable value by name
void setCWD(void); //sets the current working directory variable
void initShellVariables(void); //initialises variable storage and adds initial variables
void printAllVar(void); //prints all the variables stored in the shell
void freeAllVar(void); //frees all variables stored (freeing variable storage platform)


/* --in cmdController.c-- */

/* Constants defined */
#define MAX_ARGS 225 //maximum number of arguments made acceptable by user input

/* Methods Declarations */
void parseVrblCmd(char * args[MAX_ARGS]);
void parsePrintCmd(char * args[MAX_ARGS]);
void parseCmd(char * args[MAX_ARGS]); //makes sense of the user inputted command
void execEggShell(void); //executes the shell, considered as the main method of the eggShell