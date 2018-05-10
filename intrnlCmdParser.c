#include "eggshell.h"


//parses through recognised variable command from input command
void parseVrblCmd(char * args[MAX_ARGS]){

    char * varName = NULL; //initialized to store variable name from variable cmd
    char * varValue = NULL; //initialized to store variable value from variable cmd

    varName = strsep(&args[0], "="); //gets name before first delim "="
    varValue = args[0]; //gets whole string after first delim "="

    //if either of the entries before or after the "=" aren't filled, prompt an error
    if(varName == NULL || varValue == NULL) {

        printf("Error: Incorrect entry of a variable declaration command. ");
        printf("Please input in the form VAR_NAME=var_value\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    //if the first letter of LHS variable name is '$', prompt an error
    if(*varName == '$'){

        printf("Error: Right Hand Side Variable name shouldn't contain a $ before it's name. ");
        printf("Please input in the form VAR_NAME=var_value\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    //if dollar sign is in-front of string pointer, get value from Right-Hand Side Variable
    if(*varValue == '$'){

        varValue = getVarValue(varValue); //gets value of requested variable with dollar sign

    }

    addVar(varName, varValue); //adds variable to variables storage, or modifies value of existing variable
    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}


//parses through recognised print command from input command
void parsePrintCmd(char * args[MAX_ARGS]){

    //if nothing is to be printed
    if(args[1]==NULL){

        printf("\n"); //prints blank line when nothing is to be echoed

    }else{

        //prints each argument one by one
        for(int i = 1; args[i] != NULL; i++){

            char * after$ = args[i]+1; //gets string after $

            //if dollar sign is in-front of string pointer and there is actual string after $, get value of Right-Hand Sided Variable
            if(*args[i] == '$' && *after$ != 0){

                int flag = 0;

                char tempName[MAX_CHAR] = ""; //temporarily stores name to filter out non-variable characters
                char tempRem[MAX_CHAR] = ""; //temporarily stores remainder string for non-variable characters

                tempName[0] = '$'; //sets first character to '$'

                //goes through characters in args[i], excluding the first '$'
                for(int j = 1; j<strlen(args[i]); j++){

                    //when a variable character append to tempName
                    if (flag == 0 && isalnum(args[i][j]) || args[i][j] == '_') {

                        tempName[j] = args[i][j];

                        //when after first non-variable character reached, append to tempRem
                    }else{

                        tempRem[j-strlen(tempName)] = args[i][j];

                        flag = 1; //sets flag to 1 not to go back to appending to tempName

                    }

                }

                char tempString[MAX_CHAR] = ""; //initialized for temporary strcat() storage

                strcat(tempString, getVarValue(tempName)); //concat variable value
                strcat(tempString, tempRem); //concat remainder after this

                args[i] = tempString; //set argument to this to be printed

                printf("%s ", args[i]); //prints the current argument

            }

            //if quotation is detected at the beginning of one of the arguments
            else if(*args[i] == '\"') {

                int n = 0; //used as counter for tempArgs

                char * tempArgs[MAX_ARGS-1]; //temporary tempArgs to store what's in quotations
                args[i] = args[i] + 1; //sets pointer to after \"

                //traverses through arguments looking for closing quotation mark
                for (i; args[i] != NULL; i++) {

                    //finds argument with quotation mark in it
                    if(strstr(args[i], "\"")){

                        char tempString[MAX_CHAR] = ""; //initialized for temporary strcpy() storage
                        strcpy(tempString, args[i]); //copies contents into tempString

                        //if quotation mark not at the end of an argument or letter before the end
                        if(tempString[ strlen(args[i])-1 ] != '\"' && tempString[ strlen(args[i])-2 ] != '\"'){

                            printf("Error: Please put the terminating quotation mark at the end of your word\n");
                            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                            return;

                        }

                        //when \" appropriately at end of argument
                        else if(tempString[ strlen(args[i])-1 ] == '\"'){

                            tempString[ strlen(args[i])-1 ] = '\0'; //changes ending quotation mark to \0
                            tempArgs[n] = tempString;

                            //goes through tempArgs and prints each argument (that's in the quotations)
                            for(int l = 0; l<=n; l++){

                                printf("%s ", tempArgs[l]); //print current quotation argument

                            }

                            break;

                        }

                        //when \" appropriately at the letter before the last
                        else if(tempString[ strlen(args[i])-2 ] == '\"'){

                            tempString[ strlen(args[i])-2 ] = '\0'; //changes quotation mark before end to \0
                            tempArgs[n] = tempString;

                            //goes through tempArgs and prints each argument (that's in the quotations)
                            for(int l = 0; l<=n; l++){

                                printf("%s ", tempArgs[l]); //print current quotation argument

                            }

                            break;

                        }

                    }

                    //else when \" not found but at last argument
                    else if(args[i+1]==NULL){

                        printf("Error: No terminating quotation mark found, please finish your quote\n");
                        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                        return;

                    }

                    //else when \" not found, and not at last argument appends to tempStringArray "tempArgs"
                    else{

                        tempArgs[n] =  args[i];
                        n++; //increments n counter

                    }

                }

            }

            else { //when just a normal word

                printf("%s ", args[i]); //prints the current argument

            }

        }

        printf("\n"); //prints blank line after everything is echoed

    }

    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}


//method that parses through chdir command through args array
void parseChdirCmd(char * args[MAX_ARGS]){

    //when "chdir" on it's own is entered, set cwd as the root directory of the shell binary
    if(args[1] == NULL){

        //Note: Since is dependant on $SHELL, "chdir" doesn't work when $SHELL is manually changed
        char buffer[MAX_CHAR] = ""; //initialized to concat all other argument contents
        strcpy(buffer, getVarValue("$SHELL")); //sets buffer to SHELL variable value
        int i = (int)strlen(buffer)-1; //initialized to last character in string

        while(i --> 0 && buffer[i] != '/'){ //deletes every character after last '/'

            buffer[i] = 0;

        }

        buffer[i] = 0; //removes '/'

        chdir(buffer); //sets current working directory to initial

        setCWD(); //resets current working to initial
        setPROMPT(); //updates prompt with new cwd


    }

    //when user wants to go back out a directory
    else if(strcmp(args[1],"..") == 0 && args[2] == NULL){

        chdir(".."); //changes current directory to go up to parent
        setCWD(); //updates $CWD
        setPROMPT(); //updates $PROMPT

    }

    //when user wants to change current working directory
    else if(strcmp(args[1],"..") != 0 && args[1]!=NULL){

        char buffer[MAX_CHAR] = ""; //initialized to concat all other argument contents for path with spaces

        for(int i = 1; args[i]!=NULL; i++){

            strcat(buffer, args[i]); //concat argument

            if(args[i+1] != NULL) strcat(buffer, " "); //concat space if not at last argument

        }

        //sets to new directory and retrieves return value, 0 for success or -1 for failure
        if(chdir(buffer) == -1){

            printf("Error: The path \"%s\" was not found please change to a directory that exists\n", buffer);
            addVar("EXITCODE","-1");
            return;

        }

        setCWD(); //updates $CWD
        setPROMPT(); //updates $PROMPT

    }

    //more than one argument than ".." entered, e.g: "chdir .. as asd asd ads"
    else{

        printf("Error: Please enter one argument only for \"chdir ..\".\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}


//method that parses through source command through args array
void parseSourceCmd(char * args[MAX_ARGS]){

    //if it sees that more than one argument or no arguments is given for source
    if(args[2] != NULL){

        printf("Error: Please enter a single file name argument for \"source\".\n");
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    FILE *f; //file pointer

    //attempts to open file name string entered in args[1]
    if((f = fopen(args[1], "r")) != NULL) {

        char line[MAX_CHAR]; //initialized char array for working with fgets() and removing the character \n at the end
        char * currToken = NULL; //stores token temporarily from the current line. initialized as NULL
        char * args2[MAX_ARGS];  //array of strings used for storing tokens for current command from the .sh source file!

        int i; //counter for the token index
        int flag=0; //shows that linePntr is not as of yet malloced

        char *linePntr = NULL; //used for memory alloc, works like line for linenoise (also is freed after loop)

        //reads every line in args[1] until the end
        while(fgets(line, sizeof(args2), f) != NULL) {

            linePntr = malloc(MAX_CHAR*sizeof(char)); //initialized string pointer for use with strtok

            //removes the '\n' fgets tends to leave
            if(line[ strlen(line)-1 ] == '\n'){
                line[ strlen(line)-1 ] = '\0';
            }

            strcpy(linePntr, line); //copies line to linePntr for strtok use

            //if no characters are inputted in the line, it's entered with no content
            if(strcmp(linePntr,"") == 0){

                //Do nothing, go to next iteration

            }else{ //else when line entered with actual content

                currToken = strtok(linePntr, " "); //retrieves first token

                //if first token is set to "exit" quit the line input loop (considered a special command case)
                if (strcmp(currToken, "exit") == 0) {
                    break;
                }

                //for loop for storing currToken in array args and retrieving rest of the tokens
                for (i = 0; currToken != NULL && i < MAX_ARGS - 1; i++) {

                    args2[i] = currToken;
                    currToken = strtok(NULL, " ");

                }

                args2[i] = NULL; //set last token to NULL, useful when arguments vary per line input

                checkInputOutput(args2); //checks command for I/O redirection then parses it

                memset(line, 0, MAX_CHAR); //clears line
                flag = 1; //shows that linePntr is malloced

            }
        }

        if(flag==1) free(linePntr); //frees linePntr after malloced for the last time

    }

    //exception of retrieval failure
    else{

        printf("Error: The file \"%s\" was not found.\n", args[1]);
        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}