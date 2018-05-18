#include "eggshell.h"

pid_t suspendedPids[MAX_SUSPENDED]; //initializes stack-like array for suspended processes
int topOfTheStack = -1;

//resumes the last suspended process
void resumeSuspended(int resumeTo){

    if(topOfTheStack == -1){

        printf("Currently no suspended processes found! Can't resume what isn't paused\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }else{

        //fprintf(stdout, "Process Group: : %d\n", getpgid(current_pid));

        current_pid = suspendedPids[topOfTheStack]; //pulls pid at the top of the suspended pids Stack

        kill(current_pid, SIGCONT);  //sends SIGCONT to resume last suspended pid

        //fprintf(stdout, "Pulled Stack ID: : %d\n", topOfTheStack);
        //fprintf(stdout, "PID on resume: : %d\n", current_pid);

        suspendedPids[topOfTheStack] = 0; //set previous top to nothing
        topOfTheStack--; //decrements stack top




        //if resumed to foreground
        if(resumeTo == 1) {

            int status;
            if(waitpid(current_pid, &status, WUNTRACED) < 0){ //waits for the child process to terminate

                perror("Error: An error occurred whilst waiting for the child process.\n");
                addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                return;

            };

            //if exited normally
            if (WIFEXITED(status)) {

                addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE
                return;

            }

            else if(status == -1){ //if exited with failure case

                printf("Error: A problem went wrong in the child process during parsing external command.\n");
                addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                return;

            }

            else if(WEXITSTATUS(status)){ //if exited abnormally

                printf("Exited with status %d.\n",WEXITSTATUS(status));
                return;

            }

        }

        //if resumed to background
        else if(resumeTo == 0) {//doesn't wait therefore resumes in background
        }

    }

}

//on signal recognition, sends the signal to the current process id
void signalHandler(int signo){

    fprintf(stdout,"\n"); //prints new line safely to the terminal

    //if SIGINT is received
    if(signo == SIGINT){

        kill(current_pid, SIGINT); //sends SIGINT to interrupt current pid

    }

    //if SIGTSTP is received
    else if(signo == SIGTSTP){

        topOfTheStack++; //increments stack top
        suspendedPids[topOfTheStack] = current_pid; //pushes the current process id
        kill(current_pid, SIGTSTP); //sends SIGTSTP to interrupt current pid

        //fprintf(stdout, "Pushed Stack ID: : %d\n", topOfTheStack);
        //fprintf(stdout, "PID on pause: : %d\n", current_pid);

    }

}


void checkForSignals(void){

    struct sigaction sa; //initialize sigaction struct

    sa.sa_handler = signalHandler; //sets handler to the above signalHandler method
    sigemptyset(&sa.sa_mask); //initializes additional set of signals to be blocked during signalHandler()
    sa.sa_flags = SA_RESTART; //restarts function if interrupted by handler

    //creates signal handlers for CTRL+C and CTRL+Z
    if(sigaction(SIGINT, &sa, NULL) == -1) printf("Error: Couldn't trap CTRL+C\n");
    if(sigaction(SIGTSTP, &sa, NULL) == -1) printf("Error: Couldn't trap CTRL+Z\n");

}