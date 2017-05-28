/* CHAPTER 5 EXERCISE 6
Using the function
int waitpid(int pid, int* wstatus, int option)
In pid: we can specify the process id (negative values have a special meaning, see man wait)
wstatus: as wait
option: specifies under which conditions wait should return
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t rc = fork();

    if(rc < 0) { //error
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }

    else if(rc == 0) { //child process
        printf("Hello from child\n");
    }

    else { //rc > 0, parent process
        waitpid(rc, NULL, 0);
        printf("Hello from parent\n");
    }

    return 0;
}
