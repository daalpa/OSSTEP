/*
CHAPTER 6
In this exercise, we estimate the cost of a system call and the cost of context switching
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>


#define MIN(X,Y) ((X) < (Y)) ? (X) : (Y);
#define MAX(X,Y) ((X) > (Y)) ? (X) : (Y);



static void timerInfo(clockid_t clk_id, size_t no_readings) {

	struct timespec spec;
	if(clock_getres(CLOCK_REALTIME, &spec) < 0) {
		perror("Error reading the clock resolution");
		exit(EXIT_FAILURE);
	}
	printf("Resolution: %ld seconds and %ld nanoseconds\n", spec.tv_sec, spec.tv_nsec);

	struct timespec start, end;
	bool negative = false, zero = false;
	int64_t minimum = INT64_MAX, total = 0;

	for(size_t i = 0; i < no_readings; i++) {
			clock_gettime(clk_id, &start);
			clock_gettime(clk_id, &end);
			int64_t difference  = (end.tv_nsec - start.tv_nsec)+ (end.tv_sec - start.tv_sec)* (int64_t) 1E9; 
			total += difference;
			if(difference > 0) {
				minimum = MIN(minimum, difference);
			} else if(difference < 0) {
				negative = 1;
			}  else {
				zero = 1;
			}
	}
	if(negative) {
		printf("There were negative time readings.\n");
	}
	if(zero) {
		printf("There were time increments of size zero.\n");
	}
	printf("The smallest positive time measurement was %ld ns.\n", minimum);
	printf("The average time measurement was %f ns.\n", total/(double)no_readings);
}

/*
	The idea is simple, call getppid() several times and measure the average time per call
	getppid is called several times per iteration to account for the timer limited resolution
	and the cost of calling the timer routine.
*/

static void costOfSyscall(clockid_t clk_id, size_t no_readings)
{
	int64_t minimum = INT64_MAX;
	struct timespec start, end;
	for(size_t i = 0; i < no_readings; i++) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result" //we disable this undesired warning
			clock_gettime(clk_id, &start);
			getppid(); getppid(); getppid(); getppid(); getppid(); 
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			getppid(); getppid(); getppid(); getppid(); getppid();
			clock_gettime(clk_id, &end);
#pragma GCC diagnostic pop
			int64_t difference  = (end.tv_nsec - start.tv_nsec)+ (end.tv_sec - start.tv_sec)* (int64_t) 1E9; 
			minimum = MIN(minimum, difference);
	}
	printf("The cost of a syscall is %f ns.\n", minimum/(double) 50);
}


/*
We measure the cost of a context switch by creating two pipes connecting two processes,
a parent and its forked child. We require both processes to lie in the same processor
and processes send messages to each other. The cost of a context switch is estimated
by measuring the time required to complete a call to read, which should require
two context switches.
*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result" //we disable this undesired warning
static void costOfContextSwitch(clockid_t clk_id, size_t no_readings)
{
	enum{ READ = 0, WRITE = 1};
	int cpu = 0, ptoc[2], ctop[2];
	cpu_set_t set;
	pid_t pid;

	if(pipe(ptoc) < 0 || pipe(ctop) < 0) {
		perror("Error creating the pipes");
		exit(EXIT_FAILURE);
	}
	CPU_ZERO(&set); //empties the data structure
	CPU_SET(cpu, &set);
	
	pid = fork();
	if(pid < 0) {
		perror("The call to fork failed");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0) {
		if(sched_setaffinity(0, sizeof(set), &set) == -1) {
			perror("Failure setting the child's thread cpu affinity");
			exit(EXIT_FAILURE);
		}
		for(size_t i = 0; i < no_readings; i++) {
			char msg = 0;
			read(ptoc[READ],   &msg, sizeof(msg));
			write(ctop[WRITE], &msg, sizeof(msg));
		}
		exit(EXIT_SUCCESS);		
	}
	else {
		if(sched_setaffinity(0, sizeof(set), &set) == -1) {
			perror("Failure setting the parent's thread cpu affinity");
			exit(EXIT_FAILURE);
		}
		int64_t minimum = INT64_MAX;
		struct timespec start, end;
		for(size_t i = 0; i < no_readings; i++) {
			char msg = 0;
			clock_gettime(clk_id, &start);	
			write(ptoc[WRITE], &msg, sizeof(msg));
			read(ctop[READ],   &msg, sizeof(msg));
			clock_gettime(clk_id,&end);
			
			int64_t difference  = (end.tv_nsec - start.tv_nsec)+ (end.tv_sec - start.tv_sec)* (int64_t) 1E9; 
			minimum = MIN(minimum, difference);
		}
		wait(NULL);
		printf("The cost of a context switch is %ld ns.\n", minimum/2);
	}
}
#pragma GCC diagnostic pop

int main(void) {
	timerInfo(CLOCK_REALTIME, 1E6);
	costOfSyscall(CLOCK_REALTIME, 1E6);
	costOfContextSwitch(CLOCK_REALTIME, 1E6);
	return 0;
}
