#include "eggshell.h"

/*
void main(){

    initShellVariables();
    checkForSignals();

    char * args[MAX_ARGS] = {"top", NULL};
    checkPipeLine(args);

    signalHandler(SIGTSTP);

    char * args2[MAX_ARGS] = {"fg", NULL};
    checkCmd(args2);

}*/


int main(void){

    linenoiseClearScreen(); //clears screen and sets up CTRL+L handling for lineNoise

    printf("Booting up EggShell.................................................\n");
    printf("====================================================================}\n\n");
    printf("   ██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
    printf("   ██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
    printf("   ██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n");
    printf("   ██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n");
    printf("   ╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
    printf("   ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═══════╝\n\n");
    printf("====================================================================}\n");
    printf("---------------Type commands in the prompt below!-------------------\n\n");

    execEggShell(); //calls egg shell from cmdController.c after clearing and printing prompt
    linenoiseClearScreen(); //clears screen after eggShell is exited

    return 0;

}


//executes the REPL (Read Eval Print Loop) of the eggshell and which calls initializations then parse commands
void execEggShell(void){

    char * line; //pointer to the current line
    char * currToken = NULL; //stores token temporarily from the current line. initialized as NULL
    char * args[MAX_ARGS] = {NULL}; //array of strings used for storing tokens

    int i; //initialized counter for looping through prompting for line input

    initShellVariables(); //initializes all the shell variables in vrblController.c
    checkForSignals(); //initialized check for signals for future processes
    linenoiseHistorySetMaxLen(MAX_HISTORY); //allows ability to access previous commands using up arrow or down

    //keeps looping for line input using linenoise
    while ((line = linenoise(getVarValue("$PROMPT"))) != NULL)
    {

        //if no characters are inputted in the line, it's entered with no content
        if(strcmp(line,"") == 0){

            //Do nothing, go to next prompt iteration

        }else{ //else when line entered with actual content

            linenoiseHistoryAdd(line); //adds line to history
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
            checkPipeLine(args); //passes to check arguments for any piping, then passes it to check I/O then runs command

        }

    }

    freeAllVar(); //Frees all shell variables including the dynamic array that stores them
    linenoiseFree(line); //Frees allocated memory

}




