#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NS_IN_S 1000000000L

void usage();
void display_results(char **argv, double duration);
void display_bad_exit();
double get_duration(struct timespec start, struct timespec end);

int main(int argc, char **argv)
{
        pid_t pid;

        if (argc < 2)
                usage();

        pid = fork();

        if (pid == -1) {
                perror("fork()");
                exit(1);
        }
        else if (!pid) {                        /* Child executes program */
                execvp(argv[1], argv + 1);
                perror("exec()");
                raise(SIGINT);
        }
        else {                                  /* Parent waits and times */
                struct timespec start, end;
                double duration;                /* In seconds */
                int status;

                /* Record start time */
                if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
                        perror("clock_gettime()");
                        exit(1);
                }

                /* Wait on our child */
                waitpid(pid, &status, 0);

                /* Only display results if program exits normally */
                if (WIFEXITED(status)) {
                        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
                                perror("clock_gettime()");
                                exit(1);
                        }
                        duration = get_duration(start, end);
                        display_results(argv, duration);
                }
                else
                        display_bad_exit();
        }

        return 0;
}

void usage()
{
        fprintf(stderr, "\n\t ./time <command> [args]\n\n");
        exit(2);
}

void display_bad_exit()
{
        printf("Child process did not exit normally\n");
}

double get_duration(struct timespec start, struct timespec end)
{
        return (end.tv_sec - start.tv_sec)
                + (double)(end.tv_nsec - start.tv_nsec)
                / NS_IN_S;
}

void display_results(char **argv, double duration)
{
        char **ptr = argv + 1;
        while (*ptr) {
                printf("%s ", *ptr);
                ptr++;
        }
        printf("took %lf seconds\n", duration);
}
