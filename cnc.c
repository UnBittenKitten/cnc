#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "dynStr.h"

int hexvalue(char chr) {//assuming sanitized inputs
    if(chr >= 48 && chr <= 57) {
        return chr - 48;
    }
    if(chr >= 97 && chr <= 102) {
        return chr - 87;
    }
    return 0;
}

long long int bin_to_dec(char str[]) {
    int i = 0;
    while(i < strlen(str)) {
        if(str[i] != '1' && str[i] != '0') {
            printf("invalid binary input\n");
            return 0;
        }
        i++;
    }
    int orgSize = strlen(str) - 1;
    i = orgSize;
    long long int result = 0;
    while(i >= 0) {
        //printf("str[%d] = %c\n", i, str[i]);
        //printf("pow(2, %d) = %lf\n", orgSize - i, pow(2, orgSize - i));
        if(str[i] == '1') {
        result += pow(2, orgSize - i);
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
    int orgSize = strlen(str) - 1;
    i = orgSize;
    long long int result = 0;
    while(i >= 0) {
        //printf("str[%d] = %c\n", i, str[i]);
        result += pow(8, orgSize - i) * (int)(str[i] - 48);
        i--;
    }
    return result;
}

long long int hex_to_dec(char str[]) {
    int i = 0;
    char* newstring = (char*)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(newstring, str);
    //printf("newstring test = %s\n", newstring);
    while(i < strlen(str)) {
        //printf("str[%d] = %c\n", i, newstring[i]);
        if(newstring[i] >= 65 && newstring[i] <= 70) {
            //printf("capital\n");
            newstring[i] = newstring[i] + 32;
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
    int orgSize = strlen(newstring) - 1;
    i = orgSize;
    long long int result = 0;
    while(i >= 0) {
        //printf("newstring[%d] = %c\t", i, newstring[i]);
        //printf("pow(16, %d - %d) = %lf\n", orgSize, i, pow(16, orgSize - i));
        result += (pow(16, orgSize - i)) * (hexvalue(newstring[i]));
        i--;
    }


    free(newstring);
    return result;
}

long long int dec_to_dec(char str[]) {
    int i = 0;
    while(i < strlen(str)) {
        if(str[i] < 48 || str[i] > 57) {
            printf("invalid decimal input\n");
            return 0;
        }
        i++;
    }
    int orgSize = strlen(str) - 1;
    i = orgSize;
    long long int result = 0;
    while(i >= 0) {
        result += (pow(10, orgSize - i)) * (str[i] - 48);
        i--;
    }
    return result;
}

dynStr* dec_to_bin(long long int num) {
    dynStr* res = dyn_str_init();
    long long int new_int = num;
    int temp_int;

    while(new_int > 0) {
        temp_int = new_int % 2;
        if(temp_int == 1) {
            dyn_str_add(res, "1");
        } else {
            dyn_str_add(res, "0");
        }
        new_int /= 2;

    }
    dyn_str_rev(res);

    return res;
}

dynStr* dec_to_oct(long long int num) {
    dynStr* res = dyn_str_init();
    long long int new_int = num;
    int temp_int;

    while(new_int > 0) {
        temp_int = new_int % 8;
        dyn_str_addchar(res, temp_int + 48);
        new_int /= 8;
    }
    dyn_str_rev(res);

    return res;
}

dynStr* dec_to_hex(long long int num) {
    dynStr* res = dyn_str_init();
    long long int new_int = num;
    int temp_int;

    while(new_int > 0) {
        temp_int = new_int % 16;
        if(temp_int >= 0 && temp_int <= 9) {
            dyn_str_addchar(res, temp_int + 48);
        }
        if(temp_int >= 10) {
            dyn_str_addchar(res, temp_int + 87);
        }
        new_int /= 16;
    }
    dyn_str_rev(res);

    return res;
}



void cmd_bin_oct(char str[], int printnl) {
    dyn_str_print(dec_to_oct(bin_to_dec(str)), printnl);    
}

void cmd_bin_dec(char str[], int printnl) {
    printf("%lld", bin_to_dec(str));
    if(printnl) {
        printf("\n");
    }
}

void cmd_bin_hex(char str[], int printnl) {
    dyn_str_print(dec_to_hex(bin_to_dec(str)), printnl);
}

void cmd_oct_bin(char str[], int printnl) {
    dyn_str_print(dec_to_bin(oct_to_dec(str)), printnl);
}

void cmd_oct_dec(char str[], int printnl) {
    printf("%lld", oct_to_dec(str));
    if(printnl) {
        printf("\n");
    }
}

void cmd_oct_hex(char str[], int printnl) {
    dyn_str_print(dec_to_hex(oct_to_dec(str)), printnl);
}

void cmd_dec_bin(char str[], int printnl) {
    dyn_str_print(dec_to_bin(dec_to_dec(str)), printnl);
}

void cmd_dec_oct(char str[], int printnl) {
    dyn_str_print(dec_to_oct(dec_to_dec(str)), printnl);
}

void cmd_dec_hex(char str[], int printnl) {
    dyn_str_print(dec_to_hex(dec_to_dec(str)), printnl);
}

void cmd_hex_bin(char str[], int printnl) {
    dyn_str_print(dec_to_bin(hex_to_dec(str)), printnl);
}

void cmd_hex_oct(char str[], int printnl) {
    dyn_str_print(dec_to_oct(hex_to_dec(str)), printnl);
}

void cmd_hex_dec(char str[], int printnl) {
    printf("%lld", hex_to_dec(str));
    if(printnl) {
        printf("\n");
    }
}


int main(int argc, char* argv[]) {

    char* binamy_number = "1100110101";
    printf("Bin to Oct (%s) = ", binamy_number);
    cmd_bin_oct(binamy_number, 1);
    printf("Bin to Dec (%s) = ", binamy_number);
    cmd_bin_dec(binamy_number, 1);
    printf("Bin to Hex (%s) = ", binamy_number);
    cmd_bin_hex(binamy_number, 1);

    char* octal_number = "123151";
    printf("Oct to Bin (%s) = ", octal_number);
    cmd_oct_bin(octal_number, 1);
    printf("Oct to Dec (%s) = ", octal_number);
    cmd_oct_dec(octal_number, 1);
    printf("Oct to Hex (%s) = ", octal_number);
    cmd_oct_hex(octal_number, 1);

    char* decimal_number = "193213";
    printf("Dec to Bin (%s) = ", decimal_number);
    cmd_dec_bin(decimal_number, 1);
    printf("Dec to Oct (%s) = ", decimal_number);
    cmd_dec_oct(decimal_number, 1);
    printf("Dec to Hex (%s) = ", decimal_number);
    cmd_dec_hex(decimal_number, 1);

    char* hex_number = "dae135f";
    printf("Hex to Bin (%s) = ", hex_number);
    cmd_hex_bin(hex_number, 1);
    printf("Hex to Oct (%s) = ", hex_number);
    cmd_hex_oct(hex_number, 1);
    printf("Hex to Dec (%s) = ", hex_number);
    cmd_hex_dec(hex_number, 1);


    return 0;
}