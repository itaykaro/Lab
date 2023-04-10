#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

    FILE* input = stdin;
    FILE* output = stdout;
    
    int i, debug = 0, sign = 0, keysize, keyindex;

    for (i = 1; i < argc; i++) {
        if (debug) {
            fprintf(stderr, "%s\n", argv[i]);
        }

        if (argv[i][1] == 'D') {
            if (argv[i][0] == '+') {
                debug = 1;
            } else if (argv[i][0] == '-') {
                debug = 0;
            }
        }

        if (argv[i][1] == 'e') {
            if (argv[i][0] == '+') {
                sign = 1;
            } else if (argv[i][0] == '-') {
                sign = -1;
            }
            keyindex = i;
        }

        if (argv[i][1] == 'i' && argv[i][0] == '-') {
            input = fopen(argv[i] + 2, "r");
        }

        if (argv[i][1] == 'o' && argv[i][0] == '-') {
            output = fopen(argv[i] + 2, "w");
        }
    }

    if (sign == 0) {
        fprintf(stderr, "Invalid Key\n");
        return -1;
    }

    if (input == NULL || output == NULL) {
        fprintf(stderr, "Invalid files\n");
        return -1;
    }

    keysize = -2;
    for (i = 0; argv[keyindex][i] != 0; i++) {
        keysize++;
    }

    int key[keysize];

    for (i = 0; i < keysize; i++) {
        if (argv[keyindex][i+2] >= '0' && argv[keyindex][i+2] <= '9') {
            key[i] = argv[keyindex][i+2] - '0';
        } else {
            fprintf(stderr, "Invalid Key\n");
            return -1;
        }
    }

    int pos = 0;
    int curr, new_curr;
    do {
        curr = fgetc(input);
        if (curr == EOF) {
            break;
        }
        char range_start, range_end;
        if (curr >= '0' && curr <= '9') {
            range_start = '0';
            range_end = '9';
        } else if (curr >= 'A' && curr <= 'Z') {
            range_start = 'A';
            range_end = 'Z';
        } else if (curr >= 'a' && curr <= 'z') {
            range_start = 'a';
            range_end = 'z';
        } else {
            range_start = range_end = 0;
        } 

        if (range_start != 0) {
            new_curr = curr + (sign * key[pos]); 
            if (new_curr > range_end) {
                new_curr = new_curr - range_end + range_start - 1;
            } else if (new_curr < range_start) {
                new_curr = new_curr + range_end - range_start + 1;
            }
        } else {
            new_curr = curr;
        }

        fputc(new_curr, output);
        pos = (pos + 1) % keysize;
    } while (1);

    if (input != stdin)
        fclose(input);
    if (output != stdout)
        fclose(output);

    return 0;
}