/*************************************
* Lab 2 Exercise 3
* Name: Ngo Duc Binh
* Student Id: A0180061U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
//#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()
#include <stdbool.h>
#include <signal.h> 

char** split( char* input, char* delimiter, int maxTokenNum, int* readTokenNum )
//Assumptions:
//  - the input line is a string (i.e. with NULL character at the end)
//  - the delimiter is a string of possible delimiters, each delimiter is single chracter
//Behavior:
//  - Split up to and include maxTokenNum from the input string
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - Must use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* tStart;   //start of token
    int i, strSize;      

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    //Look for next token as separated by the delimiter
    tStart = strtok(input, delimiter); 

    i = 0;
    while (i < maxTokenNum && tStart){

        strSize = strlen(tStart);

        //Allocate space for token string. +1 for null terminator
        tokenStrArr[i] = (char*) malloc(sizeof(char) * (strSize + 1) );

        strcpy(tokenStrArr[i], tStart);    

        i++;
        tStart = strtok(NULL, delimiter); 
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}

void changePath(char **path, char *newPath) {
    *path = newPath;
}

void showPath(char *path) {
    printf("%s\n", path);
}

// for bonus exercise
int runningPid = -1;

void sighandler(int signum) {
    if (signum == SIGINT) {
        if (runningPid == -1) {
            printf("Nothing to kill!\n");
        } else {
            kill(runningPid, SIGINT);
            runningPid = -1;
        }
    }
    
}


void executeCommand(char *path, char **command, int tokenNum, int *childPids, int* childResult) {
    struct stat fileStat;
    char temp[40] = "";
    strcpy(temp, path);
    strcat(temp, "/");
    strcat(temp, command[0]);
    if (stat(temp, &fileStat) == 0) {
        int result;
        
        if (strcmp(command[tokenNum - 1], "&") != 0) {
            
            result = fork();
            if (result == 0) {
                char *args[tokenNum + 1];
                for (int i = 0; i < tokenNum; i++) {
                    args[i] = command[i];
                }
                args[tokenNum] = NULL;
                execv(temp, args);
                exit(0);
            }
            // Wait and capture the result from the child process
            runningPid = result;
            waitpid(result, childResult, 0);
            
        } else {
            // No need to wait for the child process (let it run in the background)
            result = fork();
            if (result == 0) {
                char *args[tokenNum];
                for (int i = 0; i < tokenNum - 1; i++) {
                    args[i] = command[i];
                }
                args[tokenNum - 1] = NULL;
                setpgid(0, 0);
                execv(temp, args);
                exit(0);
            } else {
                for (int i = 0; i < 10; i++) {
                    if (childPids[i] == 0) {
                        // Parent registers the child PID at the first "free" slot of the array
                        childPids[i] = result;
                        break;
                    }
                }
                printf("Child %i in background\n", result);
            }
        }
    } else {
        printf("\"%s\" not found\n", temp);
    }
}

int waitChild(int *childPids, int pid, int *childResult) {
    bool isValidPid = false;
    for (int i = 0; i < 10; i++) {
        if (pid == childPids[i]) {
            // There is a matching PID
            // Change to 0 as this child process is now waited by the parent process, reset it to 0 so that it won't be waited again
            childPids[i] = 0; 
            // Wait for the specific child
            runningPid = pid;
            isValidPid = true;
            waitpid(pid, childResult, 0);
            return 0;
        }
    }

    if (!isValidPid) {
        printf("%i not a valid child pid\n", pid);
    }

    return 0;
}

int printUnwaitedChild(int *childPids) {
    printf("Unwaited Child Processes:\n");
    for (int i = 0; i < 10; i++) {
        if (childPids[i] != 0) {
            printf("%i\n", childPids[i]);
        }
    }
    return 0;
}

int returnResult(int *childResult) {
    if (WIFEXITED(*childResult)) {
        printf("%i\n", WEXITSTATUS(*childResult));
    }
    return 0;
}

int main()
{
    char **cmdLineArgs;
    char path[20] = ".";  //default search path
    char userInput[121];
    
    int tokenNum, childResult;
    int childPids[10] = {0}; // childPids array saves the PID of all the unwaited child process

    // bonus
    signal(SIGINT, sighandler);

    // read user input
    printf("YWIMC > ");

    //read up to 120 characters from user input
    fgets(userInput, 120, stdin);

    //split is a helper function to "tokenize" the input
    //see "stringTokenizer.c" program for more sample usage 
    cmdLineArgs = split( userInput, " \n", 7, &tokenNum );

    //At this point you have the user input split neatly into token in cmdLineArg[]

    while ( strcmp( cmdLineArgs[0], "quit") != 0 ){

        //Figure out which command the user want and implement below
        if (strcmp(cmdLineArgs[0], "showpath") == 0) {
            showPath(path);
        } else if (strcmp(cmdLineArgs[0], "setpath") == 0) {
            strcpy(path, cmdLineArgs[1]);
        } else if (strcmp(cmdLineArgs[0], "wait") == 0) {
            waitChild(childPids, atoi(cmdLineArgs[1]), &childResult);
        } else if (strcmp(cmdLineArgs[0], "pc") == 0) {
            printUnwaitedChild(childPids);
        } else if (strcmp(cmdLineArgs[0], "result") == 0) {
            returnResult(&childResult);
        } else {
            executeCommand(path, cmdLineArgs, tokenNum, childPids, &childResult);
        }
        
        //Prepare for next round input

        //Clean up the token array as it is dynamically allocated
        freeTokenArray(cmdLineArgs, tokenNum);

        //Prepare for next round input
        printf("YWIMC > ");
        fgets(userInput, 120, stdin);
        cmdLineArgs = split( userInput, " \n", 7, &tokenNum );
    }
    

    printf("Goodbye!\n");

    //Clean up the token array as it is dynamically allocated
    freeTokenArray(cmdLineArgs, tokenNum);


    return 0;

}
