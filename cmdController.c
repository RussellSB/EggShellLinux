#include "eggshell.h"

//parses through inputted command, understandable through String array "args" calls methods from intrnlCmdController.c or extrnlCmdController.c
void checkCmd(char * args[MAX_ARGS]){

    //Recognises internal command as variable assignment command because first argument has an '=', and there's only one argument
    if(strstr(args[0], "=") && args[1] == NULL){

        parseVrblCmd(args); //calls method for parsing variable declaration commands

    }

    //Recognises internal command as "print" command since first argument is print
    else if(strcmp(args[0],"print") == 0){

        parsePrintCmd(args); //calls method for parsing print commands

    }

    //Recognises internal command as the "chdir" command to change the current working directory
    else if(strcmp(args[0],"chdir") == 0){

        parseChdirCmd(args); //calls method for parsing chdir commands

    }

    //Recognises internal command as "all" command therefore it prints all variables
    else if(strcmp(args[0],"all") == 0 && args[1] == NULL){

        printAllVar(); //prints all variables

    }

    //Recognises internal command as "source" command
    else if(strcmp(args[0],"source") == 0){

        parseSourceCmd(args); //parses "source" command

    }

    //Recognises internal command as "fg" command
    else if(strcmp(args[0],"fg" ) == 0 && args[1] == NULL){

        resumeSuspended(1); //resumes suspended process to foreground

    }

    //Recognises internal command as "bg" command
    else if(strcmp(args[0],"bg") == 0 && args[1] == NULL){

        resumeSuspended(0); //resumes suspended process to background

    }

    //Doesn't recognise so passes it off as an external command to handle
    else{

        externalCmd(args);

    }

}