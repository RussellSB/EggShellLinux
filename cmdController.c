#include "eggshell.h"

#define MAX_ARGS 225 //maximum number of arguments made acceptable by user input

/* This method executes the Command Line Interpreter (CLI) of the eggshell and calls initializations*/
void execEggShell(void){

    char* line;             //pointer to the current line
    char* currToken = NULL; //stores token temporarily from the current line. initialized as NULL
    char* args[MAX_ARGS];   //array of strings used for storing tokens

    int i;      //initialized counter for looping through prompting for line input

    initShellVariables(); //initializes all the shell variables in vrblController.c

    //keeps looping for line input using linenoise
    while ((line = linenoise(getVarValue("PROMPT"))) != NULL)
    {

        currToken = strtok(line, " "); //retrieves first token

        //if first token is set to "exit" quit the line input loop
        if(strcmp(currToken,"exit") == 0){
            break;
        }

        //for loop for storing currToken in array args and retrieving rest of the tokens
        for(i=0; currToken != NULL && i < MAX_ARGS-1; i++){

            args[i] = currToken;
            currToken = strtok(NULL, " ");

        }

        args[i] = NULL; //set last token to NULL, useful when arguments vary per line input

    }

    freeAllVar(1); //Frees all shell variables including the dynamic array that stores them
    linenoiseFree(line); //Frees allocated memory

}