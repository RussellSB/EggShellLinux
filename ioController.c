#include "eggshell.h"


//checks for input or output redirection
void checkInputOutput(char * args[MAX_ARGS]){

    char * cmd[MAX_ARGS-2]; //stores command

    int flag = 0; //initialized flag, changes when > or >> is detected
    int i; //initialized counter for for loop

    //traverses through arguments looking for redirection symbols
    for(i = 0; flag == 0 && args[i] != NULL; i++){

        //match found with >
        if(strcmp(args[i], ">") == 0) flag = 1;

        //match found with >>
        else if(strcmp(args[i], ">>") == 0) flag = 2;

        //match found with <
        else if(strcmp(args[i], "<") == 0) flag = 3;

        //match found with <<
        else if(strcmp(args[i], "<<<") == 0) flag = 4;

        //argument is not a I/O redirection symbol
        else cmd[i] = args[i];

    }

    //if more than one argument after > or >> is found, return an error and stop
    if( (flag == 1 || flag == 2) && args[i+1] != NULL){

        printf("Error: Please enter a single file name argument for output redirection\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    //if more than one argument after > or >> is found, return an error and stop
    else if( (flag == 3 || flag == 4) && args[i+1] != NULL){

        printf("Error: Please enter a single file name argument for input redirection\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    /* When redirection is flagged*/

    if(flag != 0){

        char * fileName = args[i]; //points to fileName

        //if valid > output redirection is detected
        if(flag == 1){

            //> stuff here
            printf("%s", fileName);


        }

        //if valid >> output redirection is detected
        else if(flag == 2){

            //>> stuff here


        }

        //if valid < input redirection is detected
        else if(flag == 3){

            //< stuff here


        }

        //if valid << input redirection is detected
        else if(flag == 4){

            //<<< stuff here


        }

    }



    /* When no redirection is flagged*/

    //if no output redirection is detected
    else{

        parseCmd(cmd); //parses through command normally with stdout

    }

}