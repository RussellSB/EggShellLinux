#include "eggshell.h"


/* Struct declarations */

//struct for storing shellVariable with it's string name and string values
typedef struct shellVariable {
    char * name;
    char * value;
} Var;


//struct for a dynamic array called VariableArray that points to ShellVariables
typedef struct variableArray {
    Var ** varArr; //double pointer to act as an array of structs
    int amount; //stores amount of variables in array, increases array dynamically
} VarArr;

/* Global variables */
VarArr * variables; //dynamic array that stores variables


//validates variable name for containing (a-z, A-Z), (0-9) or (_), returns 0 when succeeded, -1 when failed
int validateVarName(char * name) {

    char tempName[strlen(name)]; //temporarily stores name for character scanning
    strcpy(tempName, name); //copies String pointer name into String array tempName

    //scan every character in string name of variable
    for (int i = 0; i < strlen(name); i++) {

        //if character is not a digit or letter and is not an underscore
        if (!isalnum(tempName[i]) && tempName[i] != '_') {

            return -1; //return -1, indicating the name failed validation

        }

    }

    return 0; //return 0, indicating it checked every character, proved valid

}


//returns value content from variable name if exists, if doesn't returns NULL
char * getVarValue(const char * varName){

    varName+=1; //points to string after '$', so it can compare the name

    //traverses through variable array searching for a variable name match
    for(int i = 0; i < variables->amount; i++ ){

        //if a match is found
        if(strcmp(variables->varArr[i]->name,varName) == 0){

            return variables->varArr[i]->value; //return value when match is found

        }

    }

    printf("Error: Requested Shell Variable doesn't exist\n");
    addVar("EXITCODE","-1"); //exit code to -1, as error occurred
    return NULL; //return NULL when no match is found (variable doesn't exist)

}


//gets the index of a variable in the dynamic array by name
int getVarIndex(const char * varName){

    //traverses through variable array searching for a variable name match
    for(int i = 0; i < variables->amount; i++ ){

        //if a match is found
        if(strcmp(variables->varArr[i]->name,varName) == 0){
            return i; //return index when match is found
        }

    }

    return -1; //return -1 when no match is found (variable doesn't exist)

}


//method used for adding a shell variable and storing to a dynamic array of structs if doesn't already exist
void addVar(char * name, char * value) {

    if (validateVarName(name) != 0) { //used to catch just in case the variable name is not valid

        printf("Error: Variable \"%s\" should contain digits, letters and underscores only. Please try again.\n", name);

        addVar("EXITCODE","-1"); //exit code to -1, as error occurred
        return; //returns, to cancel adding variable

    }

    int v = variables->amount; //sets variable index v to the amount of variables stores

    //if at variable after the first entry
    if(v >= 1){

        int i = getVarIndex(name); //i becomes the index of varName if exists, -1 if doesn't exist

        //check if variable name is already stored in the variable storage
        if(i != -1){

            variables->varArr[i]->value = value; //since already stored just overwrites previous value with new one
            return; //returns to avoid unnecessary memory allocating

        }else{

            //dynamically increases the varArr by one as adding a new variable
            variables->varArr = (Var **) realloc(variables->varArr, sizeof(Var) * v);

        }
    }

    variables->varArr[v] = malloc(sizeof(Var)); //allocates memory for new variable in varArr[v]

    variables->varArr[v]->name = name; //sets name at index v
    variables->varArr[v]->value = value; //sets value at index v

    variables->amount++; //increases amount by 1

}


//method that sets the variable CWD for current working directory (used when resetting)
void setCWD(void){

    char buffer[MAX_CHAR];//sets buffer with max character length for string
    char * cwd; //pointer to point to this String array buffer

    getcwd(buffer, sizeof(buffer)); //gets initial current working directory

    cwd = strdup(buffer); //allocates memory for cwd, by using buffer as a source
    addVar("CWD", cwd); //adds variable
}


//sets the variable PROMPT by concatenating CWD to EggShell string and then adding $
void setPROMPT(void){

    char buffer[MAX_CHAR] = ""; //buffer declared for strcat
    char * eggString = "eggShell-lineInput";
    char * prompt; //initialized

    strcat(buffer, getVarValue("$USER")); //concat user name
    strcat(buffer, " @ ");
    strcat(buffer, eggString); //concat egg shell name
    strcat(buffer, " ~ ");
    strcat(buffer, getVarValue("$CWD")); //concat current working directory
    strcat(buffer, "$> ");

    prompt = strdup(buffer); //allocates memory for prompt, by using buffer as a source

    addVar("PROMPT", prompt); //adds variable

}


//sets the variable SHELL (Shell Variable) as path to currently executed binary file
void setSV(void){

    char pathString[MAX_CHAR]; //initialized for readlink use
    char * path = malloc(MAX_CHAR * sizeof(char)); //mallocs to avoid data corruption, imp as used for chdir

    pid_t pid = getpid(); //gets current process id to create pathString

    sprintf(pathString, "/proc/%d/exe", pid); //creates pathString for readlink()

    readlink(pathString, path, MAX_CHAR); //gets the path to executable and stores in path
    addVar("SHELL", path); //adds variable

}


//sets specific shell variables
void setShellSpecific(void){

    setSV(); //sets the path to binary file and adds as variable
    setCWD(); //current working directory, accessed by chdir and used in PROMPT
    setPROMPT(); //command prompt variable, to be used in cmdController.c
    addVar("TERMINAL", ttyname(STDIN_FILENO)); //current terminal name

}


//initializes storage platform "variables" and adds initial shell variables to storage
void initShellVariables(void) {

    //allocates memory for Variables Array which is just one item
    variables = malloc(sizeof(VarArr));

    //initializes variable amount with starting size 0, dynamically grows when adding variables
    variables->amount = 0;

    //allocating memory
    variables->varArr = calloc(1, sizeof(Var)); //initially allocates memory for 1 var in varArr

    /* Adding initial Shell Variables */

    //adding error code variable
    addVar("EXITCODE","(null)"); //initial exitcode variable

    //adding getenv() variables
    addVar("PATH", getenv("PATH")); //path to external commands
    addVar("USER", getenv("USER")); //name of current user, used in PROMPT
    addVar("HOME", getenv("HOME")); //home directory of user

    //adding shell specific variables $CWD, $PROMPT, $SHELL and $TERMINAL
    setShellSpecific(); //sets and adds them



}


//prints all the variables stored in the shell
void printAllVar(void){

    printf("\n"); //skips a line beforeexit printing all

    //traverses through storage of variables, printing out each one
    for(int i = 0; i < variables->amount; i++ ){

        printf("%s=%s\n", variables->varArr[i]->name, variables->varArr[i]->value);

    }

    printf("\n"); //skips a line after printing all
    addVar("EXITCODE","0"); //reaches here when program executes command successfully, therefore stores 0 as EXITCODE

}


//returns the size of the dynamic variables array
int getVarSize(void){

    return variables->amount;

}


//frees all the variables stored in the shell by freeing dynamic array
void freeAllVar(void){

    free(variables); //frees all variables and dynamic array that stores them

}