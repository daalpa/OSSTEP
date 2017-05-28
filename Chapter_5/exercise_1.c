/* CHAPTER 5 EXERCISE 1
Creates a variable var and a pointer to it followed by a call to fork.
We see that each process (the child and the parent) has its own copy of the variable and 
the pointer. More specifically, the processes have each their own address spaces
and modifying var in one of them won't affect the value in the other.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int var = 42;
    int *pvar = &var;
    pid_t rc = fork();

    if(rc < 0) { //error
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }

    else if(rc == 0) { //child process
        printf("CHILD: The address of var is %p\n", (void *)&var);
        printf("CHILD: The value of var is %d\n", var);
        printf("CHILD: The address stored in pvar is %p\n", (void *)pvar);
        printf("CHILD: pvar points to the value %d\n", *pvar);
        printf("CHILD: Calling var++\n");
        var++;
    }

    else { //rc > 0, parent process
        wait(NULL);
        printf("PARENT: The address of var is %p\n", (void *)&var);
        printf("PARENT: The value of var is %d\n", var);
        printf("PARENT: The address stored in pvar is %p\n", (void *)pvar);
        printf("PARENT: pvar points to the value %d\n", *pvar);
        var++;
    }

    return 0;
}
