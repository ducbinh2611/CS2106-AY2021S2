/*************************************
* Lab 2 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
--------------------------------------
Warning: Make sure your code works on
compute cluster node (Linux on x86)
*************************************/

#include <stdio.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

int main()
{
    int nChild, childResult;

    //Read the number of child
    scanf("%d", &nChild);
    int childPids[nChild];
    //Spawn child processes
    for (int i = 0; i < nChild; i++) {
        int result;
        result = fork();
        if (result == 0) {
            printf("Child %i[%i]: Hello!\n", i + 1, getpid());
            sleep(5);
            return 0;
        }
        childPids[i] = result;
    }
    //Wait on child processes in order
    
    for (int i = 0; i < nChild; i++) {
        waitpid(childPids[i], &childResult, 0);
        printf("Parent: Child %i[%i] done.\n", i + 1, childPids[i]);
    }
    return 0;
}
