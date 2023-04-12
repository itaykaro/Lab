#include <stdio.h>
#include <stdlib.h>

void PrintHex(const unsigned char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    FILE* fp;
    unsigned char* buffer;
    size_t filesize;
    
    // Check if the user provided a file name as a command line argument
    if (argc != 2) {
        printf("No file provided");
        return 1;
    }

    fp = fopen(argv[1], "rb");
    
    // find file size
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);
    
    buffer = (unsigned char*) malloc(filesize);
    
    // read file to buffer
    if (fread(buffer, sizeof(unsigned char), filesize, fp) != filesize) {
        printf("Error: Unable to read the file.\n");
        free(buffer);
        fclose(fp);
        return 1;
    }
    
    PrintHex(buffer, filesize);
    free(buffer);
    fclose(fp);
    
    return 0;
}

