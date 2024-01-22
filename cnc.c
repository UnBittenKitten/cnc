#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dynStr.h"

long long int bin_to_dec(char str[]) {
    int i = 0;
    while(i < strlen(str)) {
        if(str[i] != '1' && str[i] != '0') {
            printf("invalid binary input\n");
            return 0;
        }
        i++;
    }
    int orgsize = strlen(str) - 1;
    i = orgsize;
    long long int result = 0;
    while(i >= 0) {
        //printf("str[%d] = %c\n", i, str[i]);
        //printf("pow(2, %d) = %lf\n", orgsize - i, pow(2, orgsize - i));
        if(str[i] == '1') {
        result += pow(2, orgsize - i);
        }
        i--;
    }
    return result;
}

long long int oct_to_dec(char str[]) {
    int i = 0;
    while(i < strlen(str)) {
        if(str[i] < 48 || str[i] > 55) {
            printf("invalid octal input\n");
            return 0;
        }
        i++;
    }
    int orgsize = strlen(str) - 1;
    i = orgsize;
    long long int result = 0;
    while(i >= 0) {
        //printf("str[%d] = %c\n", i, str[i]);
        result += pow(8, orgsize - i) * (int)(str[i] - 48);
        i--;
    }
    return result;
}

int hexvalue(char chr) {//assuming sanitized inputs
    if(chr >= 48 && chr <= 57) {
        return chr - 48;
    }
    if(chr >= 97 && chr <= 102) {
        return chr - 87;
    }
    return 0;
}

long long int hex_to_dec(char* str) {
    int i = 0;
    char* newstring = (char*)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newstring, str);
    //printf("newstring test = %s\n", newstring);
    while(i < strlen(str)) {
        //printf("str[%d] = %c\n", i, newstring[i]);
        if(newstring[i] >= 65 && newstring[i] <= 70) {
            //printf("capital\n");
            *(newstring + i) = *(newstring + i) + 32;
        }
        i++;
    }
    //printf("str = %s\n", newstring);
    //printf("what\n");
    i = 0;
    while(i < strlen(newstring)) {
        //printf("newstring[%d] = %c\n", i, newstring[i]);
        if((newstring[i] < 48 || newstring[i] > 57) && (newstring[i] < 97 || newstring[i] > 102)) {
            //printf("invalid hex input\n");
            return 0;
        }
        i++;
    }
    //printf("valid string\n");
    int orgsize = strlen(newstring) - 1;
    i = orgsize;
    long long int result = 0;
    while(i >= 0) {
        //printf("newstring[%d] = %c\t", i, newstring[i]);
        //printf("pow(16, %d - %d) = %lf\n", orgsize, i, pow(16, orgsize - i));
        result += (pow(16, orgsize - i)) * (hexvalue(newstring[i]));
        i--;
    }


    free(newstring);
    return result;
}


int main(int argc, char* argv[]) {
    printf("bin dec test = %lld\n", bin_to_dec("111111111111"));
    printf("oct dec test = %lld\n", oct_to_dec("001234567"));
    // for(int i = 65; i < 91; i++) {
    //     printf("ascii[%d] = %c\n", i, i);
    // }
    // for(int i = 97; i < 123; i++) {
    //     printf("ascii[%d] = %c\n", i, i);
    // }
    //printf("\npow(16, 0) = %lf\n", pow(16, 0));
    printf("hex dec test = %lld\n", hex_to_dec("fff"));
    


    //printf("eop\n");
    return 0;
}