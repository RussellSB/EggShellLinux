#include "eggshell.h"


//parses through inputted command, understandable through String array "args"
void parseCmd(char * args[MAX_ARGS]){

    //Recognising variable command, first argument has an '=', and there's only one argument
    if(strstr(args[0], "=") && args[1] == NULL){

        char * varName = NULL; //initialized to store variable name from variable cmd
        char * varValue = NULL; //initialized to store variable value from variable cmd

        varName = strtok(args[0], "="); //gets name before delim "="
        varValue = strtok(NULL, "="); //gets value after delim "="

        printf("%s = %s\n", varName, varValue);

        //if either of the entries before or after the delim aren't filled, prompt an error
        if(varName == NULL || varValue == NULL){

            printf("Error: Incorrect entry of a variable declaration command. ");
            printf("Please input in the form VAR_NAME=var_value\n");
            return;

        }

        addVar(varName, varValue); //adds variable to variables storage, or modifies values
        //freeAllVar(0);

    }

}


//executes the Command Line Interpreter (CLI) of the eggshell and calls initializations
void execEggShell(void){

    char * line;             //pointer to the current line
    char * currToken = NULL; //stores token temporarily from the current line. initialized as NULL
    char * args[MAX_ARGS];   //array of strings used for storing tokens

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
        parseCmd(args); //parses through command input to execute the appropriate function

    }

    freeAllVar(); //Frees all shell variables including the dynamic array that stores them
    linenoiseFree(line); //Frees allocated memory

}