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

    execEggShell(); //calls egg shell from cmdController.c after clearing and printing prompt
    linenoiseClearScreen(); //clears screen after eggShell is exited

    return 0;

}




