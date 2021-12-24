#include <stdio.h>
#include <stdlib.h>
#include "read_bin.h" 

void print_bin(){
    unsigned char buffer[10];
    FILE *ptr;

    ptr = fopen("test.bin","rb");  // r for read, b for binary

    fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
    for(int i = 0; i<10; i++){
        printf("%u ", buffer[i]); // prints a series of bytes
    }
}

// int* bin_array(){
//     unsigned char buffer[10];
//     FILE *ptr;
//     int* arr;
//     arr = (int *)malloc(sizeof
// 
//     ptr = fopen("test.bin","rb");  // r for read, b for binary
// 
//     fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
//     for(int i = 0; i<10; i++){
//         
//     }
//     
// }
