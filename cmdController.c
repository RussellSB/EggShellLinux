#include "eggshell.h"

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

    //Recognises command as the chdir command to change the current working directory
    else if(strcmp(args[0],"chdir") == 0){

        parseChdrCmd(args); //calls method for parsing chdir commands

    }

    //Recognises command as "all" command therefore it prints all variables
    else if(strcmp(args[0],"all") == 0 && args[1] == NULL){

        printAllVar(); //prints all variables

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
    while ((line = linenoise(getVarValue("$PROMPT"))) != NULL)
    {

        //if no characters are inputted in the line, it's entered with no content
        if(strcmp(line,"") == 0){

            //Do nothing, go to next prompt iteration

        }else{ //else when line entered with actual content

            currToken = strtok(line, " "); //retrieves first token

            //if first token is set to "exit" quit the line input loop (considered a special command case)
            if (strcmp(currToken, "exit") == 0) {
                break;
            }

            //for loop for storing currToken in array args and retrieving rest of the tokens
            for (i = 0; currToken != NULL && i < MAX_ARGS - 1; i++) {

                args[i] = currToken;
                currToken = strtok(NULL, " ");

            }

            args[i] = NULL; //set last token to NULL, useful when arguments vary per line input
            parseCmd(args); //parses through command input to execute the appropriate function

        }

    }

    freeAllVar(); //Frees all shell variables including the dynamic array that stores them
    linenoiseFree(line); //Frees allocated memory

}