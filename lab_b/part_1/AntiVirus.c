#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

struct link {
    struct link *nextVirus;
    virus *vir;
};
typedef struct link link;


virus* readVirus(FILE* fp) {
    virus* ret;
    unsigned short sigSize;
    fread(&sigSize, 2, 1, fp);
    ret = malloc(18 + sizeof(unsigned char*));

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

void list_print(link *virus_list, FILE* output) {
    link* curr_link = virus_list;
    while(curr_link != NULL) {
        curr_link->vir = curr_link->vir;
        printVirus(curr_link->vir, output);
        curr_link = curr_link->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data) {
    link* nextVirus = NULL;
    link* newLink = malloc(sizeof(data) + sizeof(nextVirus));
    newLink->nextVirus = nextVirus;
    newLink->vir = data;

    link* curr_link = virus_list;
    if (curr_link == NULL) {
        return newLink;
    } 

    while(curr_link->nextVirus != NULL) {
        curr_link = curr_link->nextVirus;
    }
    curr_link->nextVirus = newLink;
    return virus_list;
}

void list_free(link *virus_list) {
    link* curr_link = virus_list;
    while (curr_link != NULL) {
        link* next_link = curr_link->nextVirus;
        free(curr_link->vir->sig);
        free(curr_link->vir);
        curr_link = next_link;
    }
}

link* load_signatures(char* fileName) {
    link* ret = NULL;

    char endian[4];

    long filesize;
    FILE* fp = fopen(fileName, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);

    fread(endian, 1, 4, fp);
    if (memcmp(endian, "VISL", 4) != 0) {
        fprintf(stderr, "invalid signature file\n");
        return NULL;
    }
    while (ftell(fp) != filesize) {
        ret = list_append(ret, readVirus(fp));
    }
    fclose(fp);
    return ret;
}

void detect_viruses(unsigned char *buffer, unsigned int size, link *virus_list) {
    link* curr_link = virus_list;
    while (curr_link != NULL) {
        int index = 0;
        while (index + curr_link->vir->SigSize <= size) {
            if (memcmp(buffer + index, curr_link->vir->sig, curr_link->vir->SigSize) == 0) {
                printf("Virus found!\nIn byte: %d\nVirus name: %s\nSignature size: %d\n", index, curr_link->vir->virusName, curr_link->vir->SigSize);
            }
            index++;
        }
        curr_link = curr_link->nextVirus;
    }
}

void neutralize_virus(char *fileName, int signatureOffset) {
    // return;
    FILE* fp = fopen(fileName, "rb+");
    printf("CLEANING");
    // return;
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }
    fseek(fp, 0L, SEEK_END);
    if (ftell(fp) < signatureOffset) {
        fprintf(stderr, "Invalid offset\n %ld, %d", ftell(fp), signatureOffset);
        return;
    }
    rewind(fp);
    fseek(fp, signatureOffset, SEEK_SET);
    unsigned char byte = 0xC3;
    fwrite(&byte, 1, 1, fp);
    printf("CLEANED!!! %d", signatureOffset);
    fclose(fp);
}

void fix_file(char* fileName, unsigned char *buffer, unsigned int size, link *virus_list) {
    link* curr_link = virus_list;
    while (curr_link != NULL) {
        int index = 0;
        while (index + curr_link->vir->SigSize <= size) {
            if (memcmp(buffer + index, curr_link->vir->sig, curr_link->vir->SigSize) == 0) {
                //printf("Virus found!\nIn byte: %d\nVirus name: %s\nSignature size: %d\n", index, curr_link->vir->virusName, curr_link->vir->SigSize);
                neutralize_virus(fileName, index);
            }
            index++;
        }
        curr_link = curr_link->nextVirus;
    }
}

int main(int argc, char* argv[]) { 

    FILE* fp;
    unsigned char* buffer;
    size_t filesize;
    
    // Check if the user provided a file name as a command line argument
    if (argc < 2) {
        printf("No file provided");
        return 1;
    }

    char* infectedFileNane = argv[1];

    fp = fopen(infectedFileNane, "rb");
    
    // find file size
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);
    
    buffer = (unsigned char*) malloc(10000);
    
    // read file to buffer
    if (fread(buffer, sizeof(unsigned char), filesize, fp) != filesize) {
        printf("Error: Unable to read the file.\n");
        free(buffer);
        fclose(fp);
        return 1;
    }
    fclose(fp);
    
    char* menu[] = {"Load signatures",
                    "Print signatures", 
                    "Detect viruses",
                    "Fix file",
                    "Quit"};
                
    link* virus_list = NULL;

    while(1) {

        int i;
        printf("\nSelect operation from the following menu:\n");
        for (i = 0; i < sizeof(menu) / sizeof(char*); i++) {
            printf("%i) %s\n", i+1, menu[i]);
        }
        printf("Option: ");

        char in[5];

        if (fgets(in, 5, stdin) == 0) 
            break;

        int choice = atoi(in);

        switch (choice) {
            case 1:
                char filename[100];
                fgets(filename, sizeof(filename), stdin);
                filename[strlen(filename)-1]='\0';
                virus_list = load_signatures(filename);
                break;
            
            case 2:
                list_print(virus_list, stdout);
                break;

            case 3:
                detect_viruses(buffer, filesize, virus_list);
                break;

            case 4:
                fix_file(infectedFileNane, buffer, filesize, virus_list);
                break;

            case 5:
                free(buffer);
                fclose(fp);
                return 0;

            default:
                fprintf(stderr, "Invalid choice\n");
                break;
        }
    }

    free(buffer);
    return 0;
}
