#include "eggshell.h"


//fills paths array with all possible paths for the external command
void fillPaths(char * paths[MAX_PATHS], char * fileName){

    char * pathString = getVarValue("$PATH"); //string to be split containing all possible root directories
    char * currPath = NULL; //initialized currPath

    currPath = strtok(pathString, ":"); //delimiter set as ':'

    for(int i = 0; currPath != NULL && i < MAX_PATHS; i++){

        paths[i] = malloc(MAX_CHAR * sizeof(char)); //allocates memory for paths[i]

        strcpy(paths[i], currPath);

        strcat(paths[i], "/"); //concatenation of '/'
        strcat(paths[i], fileName); //concatenation of fileBinaryName

        currPath = strtok(NULL, ":");

    }
}


//finds the valid path for the external command, if not it returns no successful path found by "-1"
char * findSuccPath(char * paths[MAX_PATHS]){

    //traverses through all possible paths
    for(int i = 0; paths[i] != NULL; i++){

        //returns valid path when found
        if(access(paths[i], X_OK) == 0) return paths[i];

    }

    return "-1"; //returns this when no successful path is found

}


//handles unrecognised command as an external command
void externalCmd(char * args[MAX_ARGS]){

    char * envp[ getVarSize() + 1]; //initializes envp String array
    fillEnvp(envp); //allocates all strings in envp appropriately (uses malloc)

    char * paths[MAX_PATHS] = {""}; //initializes array for all possible paths were binary can be stored
    fillPaths(paths, args[0]); //allocates all possible paths to filename args[0] and stores in paths

    char succPath[MAX_CHAR] = ""; //initialized successful path, that actually exists from paths
    strcpy(succPath, findSuccPath(paths));

    //checks if successful path was found or not
    if(strcmp(succPath,"-1") == 0){

        printf("Error: Path to external command \"%s\" was not found.\n", args[0]);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }else{
        //freeAllPaths(paths); //frees allocated paths in paths since successful was found
    }

    pid_t pid; //stores the current process id
    pid = fork(); //calls for a fork

    /* Command block using fork-exec pattern*/

    //accesses child
    if(pid == 0){

        execve(succPath, args, envp);

    }

    //accesses parent
    else if(pid > 0){

        int status; //initialized for storing the return status
        waitpid(pid, &status, 0); //waits for the child process to terminate

        //checks if child process failed
        if (status == -1) {

            perror("Error: An error occured when executing child process, it terminated with an error!\n");
            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
            return;

        }else{

            addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

        }

    }else{

        printf("Error: Forking was unsuccessful in extrnlCmdController.c.\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

}