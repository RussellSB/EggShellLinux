#include "eggshell.h"

pid_t suspendedPids[MAX_SUSPENDED]; //initializes stack-like array for suspended processes
int topOfTheStack = -1; //initialized index counter for the stack's top

/*
//resumes the last suspended process
void resumeSuspended(int state){

    if(topOfTheStack == -1){

        perror("Currently no suspended processes found! Can't resume what isn't paused");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }else{

        pid_t pid = suspendedPids[topOfTheStack]; //gets pid at the top of the suspended pids Stack

        kill(pid, SIGCONT);  //sends SIGCONT to resume last suspended pid

        suspendedPids[topOfTheStack] = 0; //set previous top to nothing
        topOfTheStack--; //decrements stack top

        //if resumed to foreground
        if(state == 1) {

            int status;
            waitpid(pid, &status, 0); //waits for process to finish
            return;

        }

        //if resumed to background
        else if(state == 0){

            //TODO: background stuff

        }



    }

}
 */


//on signal recognition, returns the int return value of kill, indicating success or failure
void signalHandler(int signo){

    pid_t pid = getpid(); //gets current process id

    //catches the interrupt signal CTRL+C
    if(signo == SIGINT){

        kill(pid, SIGINT); //sends SIGINT to interrupt current pid

    }

    //catches the suspend signal CTRL+Z
    else if(signo == SIGTSTP){

        suspendedPids[topOfTheStack + 1] = pid; //pushes the current process id
        topOfTheStack++; //increments stack top
        kill(pid, SIGTSTP); //sends SIGTSTP to interrupt current pid

    }

    /*NOTE: using SIGTSTP over SIGSTOP as SIGTSTP can be blocked, useful when dealing with users*/

}



void checkForSignals(void){

    struct sigaction sa; //initialize sigaction struct

    sa.sa_handler = signalHandler; //sets handler to the above signalHandler method
    sigemptyset(&sa.sa_mask); //initializes additional set of signals to be blocked during signalHandler()
    sa.sa_flags = SA_RESTART; //restarts function if interrupted by handler

    //creates signal handlers for CTRL+C and CTRL+Z
    if(sigaction(SIGINT, &sa, NULL) == -1) printf("Error: Couldn't trap CTRL+C\n");;
    if(sigaction(SIGTSTP, &sa, NULL) == -1) printf("Error: Couldn't trap CTRL+Z\n");;

}

