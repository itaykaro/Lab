#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int p[2];
    pid_t child_pid;
    pipe(p);

    if (!(child_pid = fork())) {
        close(p[0]);
        write(p[1], "hello", 5);
        close(p[1]);
        exit(0);
    } else if (child_pid == -1) {
        perror("Fork error\n");
        exit(1);
    }
    char message[5];
    close(p[1]);
    read(p[0], message, 5);
    close(p[0]);
    printf("%s\n", message);

    return 0;
}