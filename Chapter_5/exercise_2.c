/* CHAPTER 5 EXERCISE 2
Opens a file and then forks. Both processes can access the file
and the concurrent writes to the file result in interleaved
outputs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    close(STDOUT_FILENO); //close STDOUT, printf will printf will write to the file with the smallest file descriptor
    //opens the file and assigns it the smallest file descriptor possible
                         /*create    write only truncate  user read and write permission*/
    open("exercise_2.out", O_CREAT | O_WRONLY | O_TRUNC , S_IRUSR | S_IWUSR); 
    //creat("exercise_2.out",  S_IRUSR | S_IWUSR); // is equivalent to the previous line
    const int NCHAR = 10000;
    pid_t rc = fork();
    if(rc < 0) { //error
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }

    else if(rc == 0) { //child process
        printf("CHILD process, PID = %d\n", getpid());
        for(int i = 0; i < NCHAR; i++) {
            printf("c");
        }
    }

    else { //rc > 0, parent process
        printf("PARENT process, PID = %d\n", getpid());
        for(int i = 0; i < NCHAR; i++) {
            printf("p");
        }
		wait(NULL);
    }

    return 0;
}
