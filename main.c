#include "eggshell.h"

int main(int argc, char **argv){

    pid_t pid;  //used in forking processes, stores the process id
    pid = fork(); //used for forking, to display booting up prompt before booting

    if(pid == 0){

        system("clear"); //clears terminal
        printf("Booting up the Egg Shell from main.......\n"); //prompts boot up

    }else if(pid>0) {

        int returnStatus;
        wait(&returnStatus);  //parent process waits here for child to terminate.
        execEggShell(); //calls egg shell after clearing and prompt


    }else{

        printf("Error: Forking unsuccessful.");

    }

    system("clear"); //clears terminal after exiting eggShell

    return 0;
}