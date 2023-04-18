#include "LineParser.c"
#include <linux/limits.h>
#include <unistd.h>

char cwd[PATH_MAX];

void execute(cmdLine* pCmdLine) {
    execvp(pCmdLine->arguments[0], pCmdLine->arguments + 1);
    perror("Error opening file\n");
    freeCmdLines(pCmdLine);
}

int main() {
    while (1) {
        char cmd[2048];
        getcwd(cwd, sizeof(cwd));
        printf("%s/", cwd);

        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = '\0';
        cmdLine* line = parseCmdLines(cmd);
        if (strcmp(line->arguments[0], "quit") == 0) {
            break;
        }
        execute(line);
    }
    return 0;
}