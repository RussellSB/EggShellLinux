#include "eggshell.h"


//parses through recognised variable command from input command
void parseVrblCmd(char * args[MAX_ARGS]){

    char * varName = NULL; //initialized to store variable name from variable cmd
    char * varValue = NULL; //initialized to store variable value from variable cmd

    varName = strtok(args[0], "="); //gets name before delim "="
    varValue = strtok(NULL, "="); //gets value after delim "="

    //if either of the entries before or after the "=" aren't filled, prompt an error
    if(varName == NULL || varValue == NULL) {

        printf("Error: Incorrect entry of a variable declaration command. ");
        printf("Please input in the form VAR_NAME=var_value\n");
        return;

    }

    //if the first letter of variable name is '$', prompt an error
    if(*varName == '$'){

        printf("Error: Variable name shouldn't contain a $ before it's name. ");
        printf("Please input in the form VAR_NAME=var_value\n");
        return;

    }

    //
    if(*varValue == '$'){

        char * varName2 = varValue + 1; //points to letter after $, for inputting to getVarValue()
        varValue = getVarValue(varName2); //sets variable value as the value of the variable requested

    }

    addVar(varName, varValue); //adds variable to variables storage, or modifies value of existing variable

}


//parses through recognised print command from input command
void parsePrintCmd(char * args[MAX_ARGS]){

    if(args[1]==NULL){

        printf("\n"); //prints blank line when nothing is to be echoed

    }else{

        //prints each argument one by one
        for(int i = 1; args[i] != NULL; i++){

            printf("%s ", args[i]);

        }

        printf("\n"); //prints blank line after everything is echoed

    }

}

//parses through inputted command, understandable through String array "args"
void parseCmd(char * args[MAX_ARGS]){

    //Recognises command as variable assignment command because first argument has an '=', and there's only one argument
    if(strstr(args[0], "=") && args[1] == NULL){

        parseVrblCmd(args); //calls method for parsing variable declaration commands

    }

    //Recognises command as print command since first argument is print
    else if(strcmp(args[0],"print") == 0){

        parsePrintCmd(args); //calls method for parsing print commands

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