#include "eggshell.h"

/* This method executes the Command Line Interpreter (CLI) of the eggshell*/
void execEggShell(void){

    char* line;             //pointer to the current line
    char* currToken = NULL; //stores token temporarily from the current line. initialized as NULL
    char* args[MAX_ARGS];   //array of strings used for storing tokens

    int i;      //initialized counter for looping through prompting for line input

    //keeps looping for line input using linenoise
    while ((line = linenoise("eggShell-lineInput~$> ")) != NULL)
    {

        currToken = strtok(line, " "); //retrieves first token

        //if first token is set to "exit" quit the line input loop
        if(strcmp(currToken,"exit") == 0){
            break;
        }

        //for loop for storing currToken in array and retrieving rest of the tokens
        for(i=0; currToken != NULL && i < MAX_ARGS-1; i++){

            args[i] = currToken;
            currToken = strtok(NULL, " ");

        }

        args[i] = NULL; //set last token to NULL, useful when arguments vary per line input

    }

    linenoiseFree(line); // Frees allocated memory

}