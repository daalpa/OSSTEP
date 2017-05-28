/* CHAPTER 5 EXERCISE 5
Experimenting with the wait function
int wait(int* wstatus)
Waits for the completion of the child process
Returns -1, if there was an error (for instance, see below calling when there are no child processes). If wstatus != NULL, the variable 
will hold additional information about the execution of the child process. This information can be extracted using macros defined in sys/wait.h. Here, we use WIFEXITED which returns true if the process was terminated by returning from main or calling exit, while abort will cause this macro to return false. See man wait for other macros. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t pid_child = fork();

    if(pid_child < 0) { //error
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }

    else if(pid_child == 0) { //child process
        if(wait(NULL) == -1) {
            perror("Nothing to wait for! ");
        }
        abort();
        //exit(EXIT_FAILURE);
    }

    else { //rc > 0, parent process
        int wstatus = 0, pid;
        pid = wait(&wstatus);
        if(pid == -1) {
            perror("There was an error in the execution of the child process\n");
        }
        else if (pid == pid_child) {
            printf("Success\n");
            if(WIFEXITED(wstatus)) {
                printf("Exited normally\n");
            }
            else {
                printf("Exited abnormally\n");
            }
        }
    }

    return 0;
}
