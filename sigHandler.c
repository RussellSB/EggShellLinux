#include "eggshell.h"

pid_t suspendedPids[MAX_SUSPENDED]; //initializes stack for suspended processes
int topOfTheStack = -1; //initialized as -1 as the stack doesn't as of yet exist


//on signal recognition, returns the int return value of kill, indicating success or failure
void signalHandler(int signo){

    pid_t pid = getpid(); //gets current process id

    //catches the interrupt signal CTRL+C
    if(signo == SIGINT){

        if(kill(pid, SIGINT) == 0); //sends SIGINT to interrupt current pid
        else{

            perror("Sending signal SIGINT to the current process id failed");

        }

    }

    //catches the suspend signal CTRL+V
    else if(signo == SIGTSTP){

        if(kill(pid, SIGTSTP) == 0){

            suspendedPids[topOfTheStack + 1] = pid; //pushes the current process id
            topOfTheStack++; //increments stack top

        }else{

            perror("Sending signal SIGTSTP to the current process id failed");

        }

    }

    /*NOTE: using SIGTSTP over SIGSTOP as SIGTSTP can be blocked, useful when dealing with users*/

}



void checkForSignals(void){

    //....... TO DO!

}

