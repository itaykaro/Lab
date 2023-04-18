#include "LineParser.c"
#include <linux/limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

char cwd[PATH_MAX];
int debugger;

void execute(cmdLine* pCmdLine) {
    pid_t child_pid;
    if (!(child_pid = fork())) {
        if (debugger == 1) {
            fprintf(stderr, "PID: %d\nExecuting command: %s\n", getpid(), pCmdLine->arguments[0]);
        }

        // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
        if (pCmdLine->inputRedirect != NULL) {
            freopen(pCmdLine->inputRedirect, "r", stdin);
        }
        if (pCmdLine->outputRedirect != NULL) {
            freopen(pCmdLine->outputRedirect, "w+", stdout);
        }

        execvp(pCmdLine->arguments[0], pCmdLine->arguments);

        perror("Error executing command\n");
        _exit(1);
    } else if (child_pid == -1) {
        perror("Fork error\n");
    } else {
        if (pCmdLine->blocking) {
            waitpid(child_pid, NULL, 0);
        }
    }
}

int main(int argc, char** argv) {

    debugger = 0;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debugger = 1;
        }
    }
    while (1) {
        char cmd[2048];
        getcwd(cwd, sizeof(cwd));
        printf("%s$ ", cwd);

        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = '\0';
        cmdLine* line = parseCmdLines(cmd);
        if (line == NULL) {
            continue;
        }
        if (strcmp(line->arguments[0], "quit") == 0) {
            break;
        } else if (strcmp(line->arguments[0], "cd") == 0) {
            if (chdir(line->arguments[1]) == -1) {
                perror("Failed to change directory\n");
            }
        } else if (strcmp(line->arguments[0], "suspend") == 0) {
            if (line->argCount == 2) {
                if (kill(atoi(line->arguments[1]), SIGSTOP) != 0) {
                    perror("failed to send suspend signal\n");
                }
            } else {
                perror("Provide pid\n");
            }
        } else if (strcmp(line->arguments[0], "wake") == 0) {
            if (line->argCount == 2) {
                if (kill(atoi(line->arguments[1]), SIGCONT) != 0) {
                    perror("failed to send wake signal\n");
                }
            } else {
                perror("Provide pid\n");
            }
        } else if (strcmp(line->arguments[0], "kill") == 0) {
            if (line->argCount == 2) {
                if (kill(atoi(line->arguments[1]), SIGTERM) != 0) {
                    perror("failed to send kill signal\n");
                }
            } else {
                perror("Provide pid\n");
            }
        } else {
            execute(line);
        }
        freeCmdLines(line);
    }
    
    return 0;
}