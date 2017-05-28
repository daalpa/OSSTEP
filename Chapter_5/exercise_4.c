/* CHAPTER 5 EXERCISE 4
Try different forms of exec: execl, execle, execlp, execv, execve and execvp.
execl, execle, execlp: receive the arguments for the executed file through a variable argument list
execv, execve, execvp: receive the argument  for the executed file through an array of pointers to chars, i.e. char* argv[]

execle, execve: receive an additional argument specifying the environment
execlp, execvp: search for the file using the environment path variable
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_id = fork();
    if(child_id < 0) {
        perror("fork failed\n");
        exit(EXIT_FAILURE);
    }
    
    else if(child_id == 0) {
        //int rtn = execl("/bin/ls", "/bin/ls", "-a", "-l", (char*) NULL);
        int rtn = execlp("ls", "ls", "-a", "-l", (char*) NULL);
        if(rtn == -1) {
            perror("exec failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else { 
        wait(NULL);
    }
    return 0;
}
