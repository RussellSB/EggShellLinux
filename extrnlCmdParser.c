#include "eggshell.h"


//returns the environ pointer needed as one of the arguments for execve()
char** getEnviron(void){

    extern char **environ; // contains all the environment's variables

    char * terminal = malloc(MAX_CHAR * sizeof(char)); //mem allocated
    char * cwd = malloc(MAX_CHAR * sizeof(char)); //mem allocated

    //Prepares conventional string for TERMINAL
    strcat(terminal,"TERMINAL");
    strcat(terminal,"=");
    strcat(terminal,getVarValue("$TERMINAL"));

    //Prepares conventional string for CWD
    strcat(cwd,"CWD");
    strcat(cwd,"=");
    strcat(cwd,getVarValue("$CWD"));

    //Updating environ with terminal and cwd
    putenv(terminal);
    putenv(cwd);

    return environ; //returns updated environ
}


//fills paths array with all possible paths for the file name
void fillPaths(char * paths[MAX_PATHS], char * fileName){

    char * currPath = NULL; //initialized currPath

    char * pathString = getenv("PATH"); //string to be split containing all possible root directories
    char pathStrBuffer[MAX_CHAR]; //buffer used to block pointer havoc between env $PATH and strtok
    strcpy(pathStrBuffer, pathString); //copies to protect PATH from modifications by strtok

    currPath = strtok(pathStrBuffer, ":"); //delimiter set as ':'

    for(int i = 0; currPath != NULL && i < MAX_PATHS; i++){

        paths[i] = malloc(MAX_CHAR * sizeof(char)); //allocates memory for paths[i]

        strcpy(paths[i], currPath);

        strcat(paths[i], "/"); //concatenation of '/'
        strcat(paths[i], fileName); //concatenation of fileBinaryName

        currPath = strtok(NULL, ":");

    }
}


//finds the valid path for the filename, if not it returns no successful path found, by "-1"
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

    char **envp = getEnviron(); //gets the envp to pass including TERMINAL and CWD

    char * paths[MAX_PATHS] = {""}; //initializes array for all possible paths were binary can be stored
    fillPaths(paths, args[0]); //allocates all possible paths to filename args[0] and stores in paths

    char succPath[MAX_CHAR] = ""; //initialized successful path, that actually exists from paths
    strcpy(succPath, findSuccPath(paths));

    //checks if successful path was found or not
    if(strcmp(succPath,"-1") == 0){

        perror("Error: Path to external command was not found");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    pid_t pid; //stores the current process id
    pid = fork(); //calls for a fork

    /* Command block using fork-exec pattern*/

    //accesses child
    if(pid == 0){

        execve(succPath, args, envp);
        exit(0);

    }

    //accesses parent
    else if(pid > 0){

        int status; //initialized for storing the return status
        waitpid(pid, &status, 0); //waits for the child process to terminate

        //checks if child process failed
        if (status == -1) {

            perror("Error: An error occurred when executing child process.\n");
            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
            return;

        }else{

            addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

        }

    }else{

        perror("Error: Forking was unsuccessful in extrnlCmdController.c.\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

}