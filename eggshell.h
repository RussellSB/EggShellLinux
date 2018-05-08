#pragma once

/* imports used for various function calls */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "linenoise.h"
#include <ctype.h>


/* Constants defined */
#define MAX_ARGS 225 //maximum number of arguments made acceptable by user input
#define MAX_CHAR 300 //maximum number of characters in a character array (used for string buffers)
#define MAX_PATHS 30 //maximum amount of paths to parse through when calling external command


/* --in vrblController.c-- */

//Struct Declarations
typedef struct shellVariable Var; //instance of a shell variable
typedef struct variableArray VarArr; //stores multiple shell variables

//Method Declarations
int validateVarName(char* name); //check inputted varName for validity
void addVar(char* name, char* content); //adds variable to initialised variable storage
char * getVarValue(const char * variableName); //gets the variable value by name
void setCWD(void); //sets the current working directory variable
void setShellSpecific(void); //calls setCWD() and sets TERMINAL variable
void setSV(void); //sets the variable SHELL (put in other method for neatness)
void setPROMPT(void); //sets PROMPT variable using CWD
void initShellVariables(void); //initialises variable storage and adds initial variables
void printAllVar(void); //prints all the variables stored in the shell
int getVarSize(void); //returns the int of variables amount
void freeAllVar(void); //frees all variables stored (freeing variable storage platform)


/* --in intrnlCmdController.c-- */

/* Method Declarations */
void parseVrblCmd(char * args[MAX_ARGS]); //parses VARNAME=value
void parsePrintCmd(char * args[MAX_ARGS]); //parses print
void parseChdirCmd(char * args[MAX_ARGS]); //parses chdr
void parseSourceCmd(char * args[MAX_ARGS]); //parses source


/* --in extrnlCmdController.c-- */

/* Method Declarations */
void fillPaths(char * paths[ MAX_PATHS], char * fileName); //fills all possible paths, appending "/fileName"
char * findSuccPath(char * paths[MAX_PATHS]); //finds valid path from all possible paths for external command
void externalCmd(char * args[MAX_ARGS]); //gets called when command isn't recognized


/* --in cmdController.c-- */

/* Method Declarations */
void parseCmd(char * args[MAX_ARGS]); //makes sense of the user inputted command
void execEggShell(void); //executes the shell, considered as the main method of the eggShell