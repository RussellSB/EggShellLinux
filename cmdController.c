#include "eggshell.h"

//parses through inputted command, understandable through String array "args" calls methods from intrnlCmdController.c or extrnlCmdController.c
void parseCmd(char * args[MAX_ARGS]){

    //Recognises internal command as variable assignment command because first argument has an '=', and there's only one argument
    if(strstr(args[0], "=") && args[1] == NULL){

        parseVrblCmd(args); //calls method for parsing variable declaration commands

    }

    //Recognises internal command as "print" command since first argument is print
    else if(strcmp(args[0],"print") == 0){

        parsePrintCmd(args); //calls method for parsing print commands

    }

    //Recognises internal command as the "chdir" command to change the current working directory
    else if(strcmp(args[0],"chdir") == 0){

        parseChdirCmd(args); //calls method for parsing chdir commands

    }

    //Recognises internal command as "all" command therefore it prints all variables
    else if(strcmp(args[0],"all") == 0 && args[1] == NULL){

        printAllVar(); //prints all variables

    }

    //Recognises internal command as "source" command
    else if(strcmp(args[0],"source") == 0){

        parseSourceCmd(args); //parses "source" command

    }

}


//executes the REPL (Read Eval Print Loop) of the eggshell and which calls initializations then parse commands
void execEggShell(void){

    char * line;             //pointer to the current line
    char * currToken = NULL; //stores token temporarily from the current line. initialized as NULL
    char * args[MAX_ARGS];   //array of strings used for storing tokens

    int i;      //initialized counter for looping through prompting for line input

    initShellVariables(); //initializes all the shell variables in vrblController.c
    linenoiseClearScreen(); //allows ability to clear previous eggshell lines by [CTRL+L]

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