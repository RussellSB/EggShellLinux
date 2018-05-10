#include "eggshell.h"


void pipePipeLine(char * cmd1[MAX_ARGS], char * cmd2[MAX_ARGS]){

    /* Initialization and Pipe Creation */

    pid_t pid1, pid2; //process id's for producer and consumer respectively
    int fd[2]; //array of two file descriptors

    if(pipe(fd) < 0){

        perror("Error: Piping using the method pipe() has failed\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    /* Fork First Stage and wire Pipe Write End to stdout */

    if( (pid1 = fork()) < 0){

        perror("Error: In the first stage fork() has failed\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    else if(pid1 == 0){ //child

        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);

        checkInputOutput(cmd1);

    }

    /* Fork Second Stage and wire Pipe Read End to stdin */

    if( (pid2 = fork()) < 0){

        perror("Error: In the second stage fork() has failed\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    else if(pid2 == 0){ //child

        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        checkInputOutput(cmd2);

    }

    /* Closes Pipes in Parent and wait for last Pipeline stage to terminate */

    close(fd[0]);
    close(fd[1]);

    //wait for termination of last pipeline stage
    int status;
    waitpid(pid2, &status, 0);

    //checks if child process failed
    if (status == -1) {

        perror("Error: An error occurred when executing child process.\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }else{

        printf("BEEp");
        addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

    }
}


//passes command array, performing piping if more than one command contained inside it
void checkPipeLine(char * cmdArray[MAX_COMMANDS][MAX_ARGS]){


    //if no piping occurs just pass to checkInputOutput phase and execute command normally
    if(cmdArray[1][0] == NULL) checkInputOutput(cmdArray[0]);
    //if piping occurs pass to pipePipeLine phase
    else{

        int c;

        //goes through all commands in the pipeLine array "cmdArray"
        for(c = 0; cmdArray[c][0] != NULL; c++){

            //if next command is NULL, don't pipe
            if(cmdArray[c+1][0] == NULL) break;
            //else if there is a next command, pipe
            else pipePipeLine(cmdArray[c], cmdArray[c+1]);

        }

        checkInputOutput(cmdArray[c]); //executes last command normally

    }




}

//checks for any piping symbols in the input command and constructs array of tokenized commands that pipe to each other
void constructPipeLine(char * args[MAX_ARGS]){

    char * cmdArray[MAX_COMMANDS][MAX_ARGS] = {{NULL}}; //stores all commands from the pipeline (stores just 1 when no piping occurs)

    int i; //initialized counter for for loop
    int j = 0; //initialized counter for copying contents from args[i] to cmdArray
    int c = 0; //initialized counter for cmdArray

    //traverses through arguments looking for redirection symbols
    for(i = 0; args[i] != NULL; i++){

        //match found with |
        if(strcmp(args[i], "|") == 0){

            cmdArray[c][j] = NULL; //sets end to command before pipe
            j = 0; //re-initializes argument counter for next command
            c++; //increments command counter

        }

        else{

            cmdArray[c][j] = args[i];
            j++;

        }

    }

    cmdArray[c][j] = NULL; //sets end to last command without pipe | after it
    checkPipeLine(cmdArray); //passes cmdArray to parseCmdArray method for possible piping

}
