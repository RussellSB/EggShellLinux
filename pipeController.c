#include "eggshell.h"


//pipes pipeLine containing more than one command
void pipePipeLine(char * cmdArray[MAX_COMMANDS][MAX_ARGS], int cmdAmnt){

    int pAmnt = cmdAmnt - 1; //amount of pipes
    int fdAmnt = 2*pAmnt; //every pipe has two file descriptors
    int fd[pAmnt * 2]; //initializes needed file descriptors
    int status; //used for waiting in the parent
    int c = 0; //initialized new counter for commands
    int j = 0; //counter for the first fd in each pipe
    pid_t pid; //process id of the current process

    //Creates all needed pipes at the start
    for(int i = 0; i < pAmnt; i++){

        if(pipe(fd + i*2) < 0){

            perror("Error: Pipe creation has failed");
            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
            return;

        }
    }

    //traverses through commands, setting up their read and/or write in piping
    for(c; cmdArray[c][0] != NULL; c++){

        //forks the current process
        if( (pid = fork() ) < 0){

            perror("Error: forking process whilst traversing the pipeLine has failed");
            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
            return;

        }

        //accesses child
        if(pid == 0){

            //child writes to the next command if not the last command
            if(c!=cmdAmnt-1){

                //sets the current command to write to the next
                if(dup2(fd[j+1], STDOUT_FILENO) < 0){

                    perror("Error: writing process whilst traversing the pipeLine has failed");
                    addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                    exit(EXIT_FAILURE);

                }
            }

            //child reads from the previous command if not the first command
            if(c!=0){

                //sets the current command to read from the previous
                if(dup2(fd[j-2], STDIN_FILENO) < 0){

                    perror("Error: reading process whilst traversing the pipeLine has failed");
                    addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                    exit(EXIT_FAILURE);

                }
            }

            //closes the current ends of the current pipe
            for(int i = 0; i < fdAmnt; i++){
                close(fd[i]);
            }

            checkInputOutput(cmdArray[c]);
            exit(0);


        }

        j+=2;

    }

    //parent closes all the pipes in the pipeLine
    for(int i = 0; i < fdAmnt; i++){
        close(fd[i]);
    }

    //parent waits for children
    for(int i = 0; i <= pAmnt; i++){
        wait(&status);
    }

}

//checks for any piping symbols in the input command and constructs array of tokenized commands that pipe to each other
void checkPipeLine(char * args[MAX_ARGS]){

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
    c++; //increments command counter

    //if piping occurs pass to pipePipeLine phase
    if(c > 1) pipePipeLine(cmdArray, c);
        //if no piping occurs just pass first and only command to parseInputOutput phase
    else checkInputOutput(cmdArray[0]);

}