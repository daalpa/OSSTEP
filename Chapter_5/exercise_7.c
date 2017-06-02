/* CHAPTER 5 EXERCISE 7
Calls printf after closing the stdout.
The exercise asks us to do it from a child process but it seems to make no difference
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    close(STDOUT_FILENO);
    printf("Hello world!\n"); //won't be printed to screen
    perror("Called printf after closing STDOUT_FILENO");
    return 0;
}
