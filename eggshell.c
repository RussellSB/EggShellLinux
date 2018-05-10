#include "eggshell.h"

void main(){

    initShellVariables();

    char * args2[MAX_ARGS] = {"echo","twentyOneTest","|","figlet","|","figlet", NULL};
    constructPipeLine(args2);

}


/*
int main(void){

    pid_t pid;  //used in forking processes, stores the process id
    pid = fork(); //used for forking, to display booting up prompt before booting

    if(pid == 0){

        system("clear"); //clears terminal
        printf("Booting up the Egg Shell from main.......\n"); //prompts boot up

    }else if(pid>0) {

        int returnStatus;
        wait(&returnStatus);  //parent process waits here for child to terminate.

        execEggShell(); //calls egg shell after clearing and printing prompt

        printf("Exiting the Egg Shell.......\n");
        system("clear"); //clears terminal

    }else{

        printf("Error: Forking unsuccessful, could not load the Egg Shell program.");

    }

    return 0;

}

 */

