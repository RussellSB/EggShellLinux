#include "eggshell.h"


//handles unrecognised command as an external command
void externalCmd(char * args[MAX_ARGS]){

    char * envp[ getVarSize() + 1]; //initializes envp String array
    fillEnvp(envp); //allocates all strings in envp appropriately (uses malloc)

    char * paths[ MAX_PATHS]; //initializes array for all possible paths were binary can be stored



    pid_t pid; //stores the current process id
    pid = fork(); //calls for a fork

    /* Command block using fork-exec pattern*/

    //accesses child
    if(pid == 0){

        //execve

    }

    //accesses parent
    else if(pid > 0){

        int status; //initialized for storing the return status
        waitpid(pid, &status, 0); //waits for the child process to terminate

        freeAllEnvp(envp); //frees allocated strings in envp

        //returns the exit status of the terminated child for testing processes
        if (WIFEXITED(status)) {

            printf("Child exited with exitStatus = %d\n\n",WEXITSTATUS(status));

        }

        //checks if child process failed
        if (status == -1) {

            perror("Error: Could not retrieve external command, child process terminated with error!\n");
            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
            return;

        }

    }else{

        printf("Error: In looking for an external command, forking was unsuccessful.\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

}