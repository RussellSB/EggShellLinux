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

//on signal recognition, sends the signal to the current process id
void signalHandler(int signo){

    fprintf(stdout,"\n"); //prints new line safely to the terminal

    pid_t pid = getCurrentPid(); //gets current process id

    fprintf(stdout,"Handler pid: %d\n",pid); //prints new line safely to the terminal

    if(signo == SIGINT){

        int ret = kill(pid, SIGINT); //sends SIGINT to interrupt current pid
        fprintf(stdout,"ret: %d\n",ret); //prints new line safely to the terminal

    }


    else if(signo == SIGTSTP){

        int ret = kill(pid, SIGTSTP); //sends SIGTSTP to interrupt current pid
        suspendedPids[topOfTheStack + 1] = pid; //pushes the current process id
        topOfTheStack++; //increments stack top
        fprintf(stdout,"ret: %d\n",ret); //prints new line safely to the terminal

    }

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