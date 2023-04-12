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
    virus* nextVirus = NULL;
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

    unsigned char* endian[1];

    long filesize;
    FILE* fp = fopen(fileName, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    rewind(fp);

    FILE* output = fopen("text.txt", "wb"); 
    fread(endian, 4, 1, fp);
    if (strcmp(endian, "VISL") != 0) {
        fprintf(stderr, "invalid signature file\n");
        return NULL;
    }
    while (ftell(fp) != filesize) {
        ret = list_append(ret, readVirus(fp));
    }
    return ret;
}

void print_signatures(link* virus_list, FILE* output) {
    printf("not implemented\n");
}

void detect_viruses() {
    printf("not implemented\n");
}

void fix_file() {
    printf("not implemented\n");
}

int main(int argc, char* argv[]) { 

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
                detect_viruses();
                break;

            case 4:
                fix_file();
                break;

            case 5:
                return 0;

            default:
                fprintf(stderr, "Invalid choice\n");
                break;
        }
    }

    return 0;
}