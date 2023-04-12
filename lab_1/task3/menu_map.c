#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* map(char *array, int array_length, char (*f) (char)){
    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
    /* TODO: Complete during task 2.a */
    int i;
    for (i = 0; i < array_length; i++) {
        mapped_array[i] = f(array[i]);
    }
    return mapped_array;
}

char my_get(char c) {
    char in = fgetc(stdin);
    return in == c ? 0 : in;
 }


char cprt(char c) {
    if (c < 0x20 || c > 0x7E)
        printf(".\n");
    else
        printf("%c\n", c);
    return c;
}

char encrypt(char c) {
    return (c < 0x20 || c > 0x7E) ? c : c + 1;
}

char decrypt(char c) {
    return (c < 0x20 || c > 0x7E) ? c : c - 1;
}

char xprt(char c) {
    if (c < 0x20 || c > 0x7E)
        printf(".\n");
    else
        printf("%x\n", c);
    return c;
}

struct fun_desc {
    char *name;
    char (*fun)(char);
}; 

struct fun_desc menu[] = { 
    { "Get string", my_get }, 
    { "Print string", cprt }, 
    { "Encrypt", encrypt }, 
    { "Decrypt", decrypt }, 
    { "Print Hex", xprt }, 
    { NULL, NULL } 
}; 

int main(int argc, char **argv){
    int menu_size = sizeof(menu)/sizeof(menu[0]) - 1;
    char arr[] = {'\0','\0','\0','\0','\0'};
    char* carray = arr;
    while (1) {
        char in[5];
        int i = 0;
        printf("Select operation from the following menu:\n");
        for (i = 0; 1; i++) {
            if (menu[i].name == NULL) 
                break;
            printf("%i) %s\n", i, menu[i].name);
        }
        printf("Option: ");

        if (fgets(in, 5, stdin) == 0) 
            break;

        int choice = atoi(in);
        if (choice < 0 || choice > menu_size) {
            printf("Not within bounds\n");
            break;
        } 
        
        printf("Within bounds\n");
        
        carray = map(carray, 5, menu[choice].fun);
    }
}

