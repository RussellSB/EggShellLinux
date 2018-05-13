#include "eggshell.h"

/*
void main(){

    initShellVariables();
    checkForSignals();

    char * args[MAX_ARGS] = {"top", NULL};
    checkPipeLine(args);

    signalHandler(SIGTSTP);

    char * args2[MAX_ARGS] = {"fg", NULL};
    checkCmd(args2);

}*/


int main(void){

    pid_t pidMain;  //used in forking processes, stores the process id
    pidMain = fork(); //used for forking, to display booting up prompt before booting

    if(pidMain == 0){

        linenoiseClearScreen(); //clears screen and sets up CTRL+L handling for lineNoise

        printf("Booting up EggShell.................................................\n");
        printf("====================================================================}\n\n");
        printf("   ██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
        printf("   ██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
        printf("   ██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n");
        printf("   ██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n");
        printf("   ╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
        printf("   ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═══════╝\n\n");
        printf("====================================================================}\n");
        printf("---------------Type commands in the prompt below!-------------------\n\n");

    }else if(pidMain>0) {

        int status;
        waitpid(pidMain, &status, WUNTRACED);  //parent process waits here for child to terminate.

        execEggShell(); //calls egg shell after clearing and printing prompt
        system("clear"); //clears terminal

    }else{

        printf("Error: Forking unsuccessful, could not load the Egg Shell program.");

    }

    return 0;

}




