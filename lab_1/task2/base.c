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


int main(int argc, char **argv){
    /* TODO: Test your code */
    int base_len = 5;
    char arr1[base_len];
    char* arr2 = map(arr1, base_len, my_get);
    char* arr3 = map(arr2, base_len, cprt);
    char* arr4 = map(arr3, base_len, xprt);
    char* arr5 = map(arr4, base_len, encrypt);
    char* arr6 = map(arr5, base_len, decrypt);
    free(arr2);
    free(arr3);
    free(arr4);
    free(arr5);
    free(arr6); 
}

