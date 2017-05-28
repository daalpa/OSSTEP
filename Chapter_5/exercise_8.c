/* CHAPTER 5 EXERCISE 8
Creates a pipe and two processes through fork.
Process b sends a string to process a through a pipe.
Then, process a prints the string.
The pipe function receives an array of two integers.
The first integer holds the file descriptor no. to 
the output while the second position holds the file descriptor 
to the input end of the pipe.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void reader(int file_no) 
{
    char buffer[255];
    read(file_no, buffer, 255);
    puts(buffer);
}

void writer(int file_no) 
{
    char string[] = "Hello world!";
    write(file_no, string, sizeof(string)/sizeof(char));
}

int main(void)
{
    int file_no[2];
    if(pipe(file_no) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid_a;
    pid_a = fork();
    if(pid_a < 0) {
        perror("Fork failed");
    } else if(pid_a == 0) {
        reader(file_no[0]);
    } else {
        pid_t pid_b = fork();
        if(pid_b < 0) {
            perror("Fork failed");
        } else if(pid_b == 0) {
            writer(file_no[1]);
        }
    }    
    return 0;
}
