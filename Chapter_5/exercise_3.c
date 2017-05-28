/* CHAPTER 5 EXERCISE 3
This exercise asks us to create a program where the child process prints a
message before the parent process without using the system call wait.
The system function sleep is used to put the parent to sleep for 1 second. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t child_id = fork();

    if(child_id < 0) {
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }

    else if(child_id == 0) { //child process
        printf("Hello!\n");
    }
    else { //parent process
        sleep(1);
        printf("Good bye!\n");
    }
    return 0;
}
