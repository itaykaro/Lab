#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int p[2];
    pid_t child1, child2;

    if (pipe(p) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr,  "(parent_process>forking child1)\n");
    // fork first child
    child1 = fork();
    if (child1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {
        // child1 process
        fprintf(stderr,  "(child1>redirecting stdout to the write end of the pipe…)\n");
        close(STDOUT_FILENO);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        char * const line1[3]  =  {"ls", "-l", NULL};
        fprintf(stderr,  "(child1>going to execute cmd: ls -l)\n");
        execvp(line1[0], line1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr,  "(parent_process>created process with id: %d)\n", child1);

    // close write end of the pipe in the parent process
    fprintf(stderr,  "(parent_process>closing the write end of the pipe…)\n");
    close(p[1]);

    fprintf(stderr,  "(parent_process>forking child2)\n");
    // fork second child
    child2 = fork();
    if (child2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) {
        // child2 process
        fprintf(stderr,  "(child2>redirecting stdin to the read end of the pipe…)\n");
        close(STDIN_FILENO);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        char* const line2[4] = {"tail", "-n", "2", NULL};
        fprintf(stderr,  "(child2>going to execute cmd: tail -n 2)\n");
        execvp(line2[0], line2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr,  "(parent_process>created process with id: %d)\n", child2);

    // close read end of the pipe in the parent process
    fprintf(stderr,  "(parent_process>closing the read end of the pipe…)\n");

    close(p[0]);

    // wait for child processes to terminate
    fprintf(stderr,  "(parent_process>waiting for child processes to terminate…)\n");

    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    fprintf(stderr,  "(parent_process>exiting…)\n");
    return 0;
}

