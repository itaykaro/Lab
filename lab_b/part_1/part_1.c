#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

virus* readVirus(FILE* fp) {
    virus* ret;
    unsigned short sigSize;
    fread(&sigSize, 2, 1, fp);
    ret = malloc(18 + sigSize);

    ret->SigSize = sigSize;
    fread(ret->virusName, 16, 1, fp);
    ret->sig = malloc(ret->SigSize);
    fread(ret->sig, ret->SigSize, 1, fp);

    return ret;
}

void printVirus(virus* virus, FILE* output) {
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature: \n", virus->virusName, virus->SigSize);
    for (int i = 0; i < virus->SigSize; i++) {
        fprintf(output, "%02X ", virus->sig[i]);
        if ((i + 1) % 20 == 0 || i == virus->SigSize - 1) {
            fprintf(output, "\n");
        }
    }
    fprintf(output, "\n");
}

int main(int argc, char* argv[]) { 
    unsigned char* endian[1];
    long filesize;
    FILE* fp = fopen("signatures-L", "rb");
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);
    FILE* output = fopen("text.txt", "wb");
    fread(endian, 4, 1, fp);
    if (strcmp(endian, "VISL") != 0) {
        fprintf(stderr, "invalid signature file");
        return 1;
    }
    while (ftell(fp) != filesize) {
        printVirus(readVirus(fp), output);
    }
    return 0;
}
