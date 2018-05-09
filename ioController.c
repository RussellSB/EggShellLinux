#include <fcntl.h>
#include "eggshell.h"


//checks for input or output redirection
void redirectOutput(char * cmd[MAX_ARGS], char * fileName, int flag){

    /* Connect standard output to given file*/

    fflush(stdout); //used as a safety precaution

    int fd1 = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644); //sets file descriptor for file

    if(fd1 < 0){

        printf("Error: Failed to open the file \"%s\"\n for writing\n", fileName);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    int fd2 = dup(STDOUT_FILENO); //sets output to file descriptor instead of stdout

    if(fd2 < 0){

        printf("Error: Failed to duplicate standard output\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    //restores fd1
    if(dup2(fd1, STDOUT_FILENO) < 0){

        printf("Error: Failed to duplicate the file \"%s\" to standard output\n", fileName);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    close(fd1);

    /* Write to standard output */

    parseCmd(cmd); //parses command

    /* Reconnect original standard output */

    fflush(stdout); //safety precaution

    //restores fd2
    if(dup2(fd2, STDOUT_FILENO) < 0){

        printf("Error: Failed to reinstate standard output\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    close(fd2);
    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}


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

        //if valid > output redirection is detected
        if(flag == 1){

            //> stuff here
            redirectOutput(cmd, args[i], flag);


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