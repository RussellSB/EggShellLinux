#include "eggshell.h"

/*
void main(){

    initShellVariables();
    char * args[MAX_ARGS] = {"chdir",".."};
    parseCmd(args);
    char * args2[MAX_ARGS] = {"print","$CWD"};
    parseCmd(args2);

}*/


int main(int argc, char **argv){

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

        printf("Error: Forking unsuccessful.");

    }

    return 0;

}


