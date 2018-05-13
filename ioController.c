#include "eggshell.h"

//stdin set to point to fileName
int stdinToFile(char * fileName){

    fflush(stdin); //used as a safety precaution

    int fd1; //initialized file descriptor 1

    //sets file descriptor for file for reading file
    fd1 = open(fileName, O_RDONLY);

    if(fd1 < 0){

        printf("Error: Failed to open the file \"%s\" for reading\n", fileName);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return -1;

    }

    int fd2 = dup(STDIN_FILENO); //sets input to file descriptor instead of stdin

    if(fd2 < 0){

        printf("Error: Failed to duplicate standard input\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return -1;

    }

    //restores fd1
    if(dup2(fd1, STDIN_FILENO) < 0){

        printf("Error: Failed to duplicate the file \"%s\" for standard input\n", fileName);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return -1;

    }

    close(fd1);
    return fd2;

}

//resets stdin back to normal using pointer made in stdinToFile()
void stdinToNormal(int fd2){

    fflush(stdin); //safety precaution

    //restores fd2
    if(dup2(fd2, STDIN_FILENO) < 0){

        printf("Error: Failed to reinstate standard input\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    close(fd2);

}


//redirects stdin to string by tempFile "hereString.tmp," catering for symbol '<<<'
void redirectInputString(char * cmd[MAX_ARGS], char * stringArray[MAX_ARGS]){

    /* Creates temporary file and stores string */

    char * fileName = "hereString.tmp";
    FILE *f = fopen("hereString.tmp", "w+");

    for(int i = 0; stringArray[i] != NULL; i++){

        if(stringArray[i+1] == NULL)fprintf(f, "%s", stringArray[i]);
        else fprintf(f, "%s ", stringArray[i]);

    }

    fclose(f);

    /* Connect standard input to given file*/
    int fd2 = stdinToFile(fileName); //returns fd2 for later use for reversion

    //if -1 is returned, error occurred and printed so just return
    if(fd2 == -1){
        return;
    }

    /* Write to standard input */
    checkCmd(cmd); //parses command

    /* Reconnect original standard input */
    stdinToNormal(fd2);

    remove(fileName); //removes temporary file

}


//redirects stdin to newly created file "fileName," catering for symbol '<'
void redirectInputFile(char * cmd[MAX_ARGS], char * fileName){

    /* Connect standard output to given file*/

    int fd2 = stdinToFile(fileName); //returns fd2 for later use for reversion

    //if -1 is returned, error occurred and printed so just return
    if(fd2 == -1){
        return;
    }

    /* Write to standard input */
    checkCmd(cmd); //parses command

    /* Reconnect original standard input */
    stdinToNormal(fd2);

}


//sets stdout to point to fileName
int stdoutToFile(char * fileName, int flag){

    fflush(stdout); //used as a safety precaution

    int fd1; //initialized file descriptor 1

    //sets file descriptor for file for creating file
    if(flag == 1) fd1 = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        //sets file descriptor for appending to file when flag is 2
    else fd1 = open(fileName, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if(fd1 < 0){

        printf("Error: Failed to open the file \"%s\" for writing\n", fileName);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return -1;

    }

    int fd2 = dup(STDOUT_FILENO); //sets output to file descriptor instead of stdout

    if(fd2 < 0){

        printf("Error: Failed to duplicate standard output\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return -1;

    }

    //restores fd1
    if(dup2(fd1, STDOUT_FILENO) < 0){

        printf("Error: Failed to duplicate the file \"%s\" to standard output\n", fileName);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return -1;

    }

    close(fd1);
    return fd2;

}


void stdoutToNormal(int fd2){

    fflush(stdout); //safety precaution

    //restores fd2
    if(dup2(fd2, STDOUT_FILENO) < 0){

        printf("Error: Failed to reinstate standard output\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    close(fd2);

}

//redirects stdout to newly created file "fileName," catering for symbol '>'
void redirectOutput(char * cmd[MAX_ARGS], char * fileName, int flag){

    /* Connect standard output to given file*/
    int fd2 = stdoutToFile(fileName, flag);

    /* Write to standard output */
    checkCmd(cmd); //checks command type, then parses and executes

    /* Reconnect original standard output */
    stdoutToNormal(fd2);

}


//checks for input or output redirection
void checkInputOutput(char * args[MAX_ARGS]){

    char * cmd[MAX_ARGS] = {NULL}; //stores command

    int flag = 0; //initialized flag, changes when > or >> is detected
    int i; //initialized counter for for loop

    //traverses through arguments looking for redirection symbols
    for(i = 0; flag == 0 && args[i] != NULL; i++){

        //match found with >
        if(strcmp(args[i], ">") == 0) flag = 1;

        //match found with >>
        else if(strcmp(args[i], ">>") == 0) flag = 2;

        //match found with <
        else if(strcmp(args[i], "<") == 0) flag = 3;

        //match found with <<
        else if(strcmp(args[i], "<<<") == 0) flag = 4;

        //argument is not a I/O redirection symbol
        else cmd[i] = args[i];

    }

    //if more than one argument after > or >> is found, return an error and stop
    if( (flag == 1 || flag == 2) && args[i+1] != NULL){

        printf("Error: Please enter a single file name argument for output redirection\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    //if more than one argument after <, return an error and stop
    else if( (flag == 3) && args[i+1] != NULL){

        printf("Error: Please enter a single file name argument for input redirection\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    /* When redirection is flagged*/

    if(flag != 0){

        //if output redirection is detected
        if(flag == 1 || flag == 2) redirectOutput(cmd, args[i], flag);

        //file input redirection '<' is detected
        else if(flag == 3) redirectInputFile(cmd, args[i]);

        //string input redirection '<<<' is detected (flag == 4)
        else{

            char * stringArray[MAX_ARGS]; //makes string array for string after "<<<"

            for(int j = 0; args[i] != NULL; j++){

                stringArray[j] = args[i]; //puts all contents into stringArray
                i++;

            }

            stringArray[i] = NULL; //sets last token to null
            redirectInputString(cmd, stringArray);

        }

    }

    /* When no redirection is flagged*/

    //if no output redirection is detected
    else checkCmd(cmd); //parses through command normally with stdout

}