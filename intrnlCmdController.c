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

                char * tempArgs[MAX_ARGS]; //temporary tempArgs to store what's in quotations
                tempArgs[0] = args[i]+1; //stores first argument from quotation (excluding quotation mark)
                int n = 1; //counter for tempArgs[] appending in else

                //traverses through rest of arguments looking for closing quotation mark
                for (int k = i+1; args[k] != NULL; k++) {

                    //finds next argument with quotation mark in it
                    if(strstr(args[k], "\"")){

                        char tempString[MAX_CHAR] = ""; //initialized for temporary strcpy() storage
                        strcpy(tempString, args[k]); //copies contents into tempString

                        //if quotation mark not at the end of an argument
                        if(tempString[ strlen(args[k])-1 ] != '\"'){

                            printf("Error: Please put ending quotation mark at the end of your argument\n");

                            addVar("EXITCODE","-1"); //exit code to -1, as error occurred
                            return;

                        }else{ //when appropriately at end of argument

                            tempString[ strlen(args[k])-1 ] = '\0'; //changes ending quotation mark to \0
                            tempArgs[n] = tempString;

                            //goes through tempArgs and prints each argument (that's in the quotations)
                            for(int l = 0; l<=n; l++){

                                printf("%s ", tempArgs[l]); //print current quotation argument

                            }

                            i = k; //set i to after argument with quotation mark
                            break;


                        }

                    }else{ //else appends to tempStringArray "tempArgs"

                        tempArgs[n] =  args[k];
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


//method that parses through chdr command through args array
void parseChdrCmd(char * args[MAX_ARGS]){

    //when just cmd "cd" on it's own is entered
    if(args[1] == NULL){

        setCWD(); //resets current working to initial
        setPROMPT(); //updates prompt with new cwd

    }

    //when user wants to go back out a directory
    else if(strcmp(args[1],"..") == 0 && args[2] == NULL){

        char buffer[MAX_CHAR] = ""; //initialized for strcat()
        char * cwd;

        strcat(buffer, getVarValue("$CWD")); //tempString buffer becomes value of $CWD

        int i = (int)strlen(buffer)-1; //initialized to last character in string
        while(i --> 0 && buffer[i] != '/'){ //deletes every character after last '/'

            buffer[i] = 0;

        }

        buffer[i] = 0; //removes '/'
        cwd = strdup(buffer); //allocates memory for cwd, by using buffer as a source

        addVar("CWD",cwd); //updates new current working directory
        setPROMPT(); //updates prompt with new cwd

    }

    //when user wants to change current working directory
    else if(strcmp(args[1],"..") != 0 && args[1]!=NULL){

        char buffer[MAX_CHAR] = ""; //initialized tp concat all other argument contents
        char * cwd;

        for(int i = 1; args[i]!=NULL; i++){

            strcat(buffer, args[i]); //concat argument
            strcat(buffer, " "); //concat space

        }

        cwd = strdup(buffer);

        addVar("CWD", cwd); //updates new current working directory
        setPROMPT(); //updates prompt with new cwd

    }

    //more than one argument for chdir entered
    else{

        printf("Error: Please put just one argument for chdir.\n");

        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return;

    }

    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}