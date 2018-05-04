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

/* in vrblController.c */

//Struct Declarations
typedef struct shellVariable Var;
typedef struct variableArray VarArr;

//Method Declarations
int validateVarName(char* name);
void addVar(char* name, char* content);
char * getVarValue(const char * variableName);
void initShellVariables(void);
void printAllVar(void);
void freeAllVar(int x);


/* in cmdController.c */

/* Methods Declarations */
void execEggShell(void);