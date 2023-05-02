#include "LineParser.c"
#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

char cwd[PATH_MAX];
int debugger;

void execute(cmdLine* pCmdLine) {
    pid_t child_pid;
    if(pCmdLine->next == NULL){
        if(pCmdLine->next==NULL){
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
    }
    else{
        pid_t child1, child2;
        int p[2];
        if (pipe(p) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
            }
            child1 = fork();
            if (child1 == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (child1 == 0) {
                // child1 process
                if (debugger == 1) {
                    fprintf(stderr, "PID: %d\nExecuting command: %s\n", getpid(), pCmdLine->arguments[0]);
                }

                // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                if (pCmdLine->inputRedirect != NULL) {
                    freopen(pCmdLine->next->inputRedirect, "r", stdin);
                    exit(EXIT_FAILURE);
                }
                if (pCmdLine->outputRedirect != NULL) {
                    fprintf(stderr,"Cannot change output to left side of the pipe\n");
                    exit(EXIT_FAILURE);
                }
                close(STDOUT_FILENO);
                dup(p[1]);
                close(p[0]);
                close(p[1]);
                execvp(pCmdLine->arguments[0], pCmdLine->arguments);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            // close write end of the pipe in the parent process
            close(p[1]);

            // fork second child
            child2 = fork();
            if (child2 == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            
            if (child2 == 0) {
                // child2 process
                if (debugger == 1) {
                    fprintf(stderr, "PID: %d\nExecuting command: %s\n", getpid(), pCmdLine->next->arguments[0]);
                }

                // https://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm
                if (pCmdLine->next->inputRedirect != NULL) {
                    fprintf(stderr, "Cannot change input to right side of the pipe\n");
                    exit(EXIT_FAILURE);
                }
                if (pCmdLine->next->outputRedirect != NULL) {
                    freopen(pCmdLine->outputRedirect, "w+", stdout);
                }
                close(STDIN_FILENO);
                dup(p[0]);
                close(p[0]);
                close(p[1]);
                execvp(pCmdLine->next->arguments[0], pCmdLine->next->arguments);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            if(pCmdLine->next->next != NULL){
                execute(pCmdLine->next->next);
            }
            if(pCmdLine->blocking) {
                waitpid(child1, NULL, 0);
            }
            if(pCmdLine->next->blocking) {
                waitpid(child2, NULL, 0);
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