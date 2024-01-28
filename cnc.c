//Version 1.0.1

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
//Future functionality
//Add trailing 0s for binary numbers
//Also separate binary numbers in groups of 4
//Floating point numbers
//Transform between the bases (2 - 36)
//If posible add arbitrary big numbers
#include "libs/dynStr.h"
#include "libs/gmp.h"
#include "libs/limits.h"
#include "libs/stddef.h"
#include "libs/syslimits.h"

int hexvalue(char chr) {
    if(chr >= 48 && chr <= 57) {
        return chr - 48;
    }
    if(chr >= 97 && chr <= 102) {
        return chr - 87;
    }
    return 0;
}

void removecaps(dynStr* dyn_str) {
    int i = 0;
    while(i < strlen(*dyn_str->str)) {
        //printf("str[%d] = %c\n", i, newstring[i]);
        if((*dyn_str->str)[i] >= 65 && (*dyn_str->str)[i] <= 70) {
            //printf("capital\n");
            (*dyn_str->str)[i] = (*dyn_str->str)[i] + 32;
        }
        i++;
    }
}

int strval1(char str[]) {
    if(!strcmp(str, "bin")) return 0;
    if(!strcmp(str, "oct")) return 0;
    if(!strcmp(str, "dec")) return 0;
    if(!strcmp(str, "hex")) return 0;
    return 1;
}

int strval2(char str[]) {
    if(!strcmp(str, "bin")) return 0;
    if(!strcmp(str, "oct")) return 0;
    if(!strcmp(str, "dec")) return 0;
    if(!strcmp(str, "hex")) return 0;
    if(!strcmp(str, "all")) return 0;
    return 1;
}

int bin_val(dynStr* dyn_str) {
    int i = 0;
    while(i < strlen((*dyn_str->str))) {
        if((*dyn_str->str)[i] != '1' && (*dyn_str->str)[i] != '0') {
            //printf("\naaaaaaaaInvalid binary input");
            //printf("\nreturning 0\n");
            return 0;
        }
        i++;
    }
    //printf("\nreturning 1\n");
    return 1;
}

int oct_val(dynStr* dyn_str) {
    int i = 0;
    while(i < strlen((*dyn_str->str))) {
        if((*dyn_str->str)[i] < 48 || (*dyn_str->str)[i] > 55) {
            //printf("Invalid octal input\n");
            return 0;
        }
        i++;
    }
    return 1;
}

int dec_val(dynStr* dyn_str) {
    int i = 0;
    while(i < strlen((*dyn_str->str))) {
        if((*dyn_str->str)[i] < 48 || (*dyn_str->str)[i] > 57) {
            //printf("invalid decimal input\n");
            return 0;
        }
        i++;
    }
    return 1;
}

int hex_val(dynStr* dyn_str) {
    int i = 0;
    while(i < strlen((*dyn_str->str))) {
        //printf("newstring[%d] = %c\n", i, newstring[i]);
        if(((*dyn_str->str)[i] < 48 || (*dyn_str->str)[i] > 57) && ((*dyn_str->str)[i] < 97 || (*dyn_str->str)[i] > 102)) {
            //printf("invalid hex input\n");
            return 0;
        }
        i++;
    }

    return 1;
}


long long int bin_to_dec(dynStr* dyn_str) {
    int orgSize = strlen(*(dyn_str->str)) - 1;
    int i = orgSize;
    long long int result = 0;
    
    while(i >= 0) {
        //printf("\n");
        if(*(*dyn_str->str + i) == '1') {
        result += pow(2, orgSize - i);
        }
        i--;
    }
    return result;
}

long long int oct_to_dec(dynStr* dyn_str) {
    int orgSize = strlen(*(dyn_str->str)) - 1;
    int i = orgSize;
    long long int result = 0;
    while(i >= 0) {
        //printf("str[%d] = %c\n", i, str[i]);
        result += pow(8, orgSize - i) * (int)(*(*dyn_str->str + i) - 48);
        i--;
    }
    return result;
}

long long int hex_to_dec(dynStr* dyn_str) {
    int orgSize = strlen(*(dyn_str->str)) - 1;
    int i = orgSize;
    long long int result = 0;
    while(i >= 0) {
        //printf("newstring[%d] = %c\t", i, newstring[i]);
        //printf("pow(16, %d - %d) = %lf\n", orgSize, i, pow(16, orgSize - i));
        result += (pow(16, orgSize - i)) * (hexvalue(*(*dyn_str->str + i) ));
        i--;
    }


    return result;
}

long long int dec_to_ll(dynStr* dyn_str) {
    int orgSize = strlen(*(dyn_str->str)) - 1;
    int i = orgSize;
    long long int result = 0;

    while(i >= 0) {
        result += pow(10, orgSize - i) * (*(*dyn_str->str + i) - 48);
        i--;
    }
    return result;
}


void dec_to_bin(dynStr** dyn_str, long long int num) {
    long long int new_int = num;
    int temp;

    //dyn_str_del(*(dyn_str));
    dyn_str_clear(*dyn_str, 0);
    while(new_int > 0) {
        temp = new_int % 2;
        if(temp == 1) {
            dyn_str_add(*(dyn_str), "1");
        } else {
            dyn_str_add(*(dyn_str), "0");
        }
        new_int /= 2;
    }

    dyn_str_rev(*(dyn_str));
}

void dec_to_oct(dynStr** dyn_str, long long int num) {
    long long int new_int = num;
    int temp_int;

    while(new_int > 0) {
        temp_int = new_int % 8;
        dyn_str_addchar(*(dyn_str), temp_int + 48);
        new_int /= 8;
    }
    dyn_str_rev(*(dyn_str));
}

void dec_to_hex(dynStr** dyn_str, long long int num) {
    long long int new_int = num;
    int temp_int;

    while(new_int > 0) {
        temp_int = new_int % 16;
        if(temp_int >= 0 && temp_int <= 9) {
            dyn_str_addchar(*(dyn_str), temp_int + 48);
        }
        if(temp_int >= 10) {
            dyn_str_addchar(*(dyn_str), temp_int + 87);
        }
        new_int /= 16;
    }

    dyn_str_rev(*(dyn_str));
}


void out_bin_oct(dynStr** result, dynStr* holder, int newline) {
    if(bin_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_oct(result, bin_to_dec(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVbin");
        if(newline) printf("\n");
    }
}

void out_bin_dec(dynStr** result, dynStr* holder, int newline) {
    if(bin_val(holder)) {
        printf("%lld", bin_to_dec(holder));
    } else {
        printf("NVbin");
    }
        if(newline) printf("\n");
}

void out_bin_hex(dynStr** result, dynStr* holder, int newline) {
    if(bin_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_hex(result, bin_to_dec(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVbin");
        if(newline) printf("\n");
    }
}

void out_oct_bin(dynStr** result, dynStr* holder, int newline) {
    if(oct_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_bin(result, oct_to_dec(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVoct");
        if(newline) printf("\n");
    }
}

void out_oct_dec(dynStr** result, dynStr* holder, int newline) {
    if(oct_val(holder)) {
        printf("%lld", oct_to_dec(holder));
    } else {
        printf("NVoct");
    }
        if(newline) printf("\n");
}

void out_oct_hex(dynStr** result, dynStr* holder, int newline) {
    if(oct_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_hex(result, oct_to_dec(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVoct");
        if(newline) printf("\n");
    }
}

void out_dec_bin(dynStr** result, dynStr* holder, int newline) {
    if(dec_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_bin(result, dec_to_ll(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVdec");
        if(newline) printf("\n");
    }
}

void out_dec_oct(dynStr** result, dynStr* holder, int newline) {
    if(dec_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_oct(result, dec_to_ll(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVdec");
        if(newline) printf("\n");
    }
}

void out_dec_hex(dynStr** result, dynStr* holder, int newline) {
    if(dec_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_hex(result, dec_to_ll(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVdec");
        if(newline) printf("\n");
    }
}

void out_hex_bin(dynStr** result, dynStr* holder, int newline) {
    if(hex_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_bin(result, hex_to_dec(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVhex");
        if(newline) printf("\n");
    }
}

void out_hex_oct(dynStr** result, dynStr* holder, int newline) {
    if(hex_val(holder)) {
        dyn_str_clear(*result, 0);
        dec_to_oct(result, hex_to_dec(holder));
        dyn_str_print(*result, newline);
    } else {
        printf("NVhex");
        if(newline) printf("\n");
    }
}

void out_hex_dec(dynStr** result, dynStr* holder, int newline) {
    if(hex_val(holder)) {
        printf("%lld", hex_to_dec(holder));
    } else {
        printf("NVhex");
    }
    if(newline) printf("\n");
}



int main(int argc, char* argv[]) {
    dynStr** result = (dynStr**)malloc(sizeof(dynStr**));
    *result = dyn_str_init();
    dynStr* holder = dyn_str_init();
    
    if(argc == 1) {
        printf("Type \"cnc -h\" for help\n");
        return 0;
    }

    if(!strcmp(argv[1], "-h") && argc == 2) {
        printf("Type cnc and then two of the following\n");
        printf("keywords without repeating: bin, oct, dec, hex\n");
        printf("The program will transform the numbers\n");
        printf("you input from the base specified\n");
        printf("first to the base you specified second.\n");
        printf("Aditionally, if you type one of those keywords\n");
        printf("followed by \"all\" the program will output\n");
        printf("the numbers you input in the base specified\n");
        printf("to all of the available numeric bases\n");
        return 0;
    }

    if(argc < 4) {
        printf("Not enough arguments\n");
        return 0;
    }

    if(strval1(argv[1])) {
        printf("Invalid first argument, \"%s\" is not a valid numeric base\n", argv[1]);
        printf("For help type \"cnc -h\"\n");
        return 0;
    }

    if(strval2(argv[2])) {
        printf("Invalid second argument, \"%s\" is not a valid numeric base\n", argv[2]);
        printf("For help type \"cnc -h\"\n");
        return 0;
    }

    if(!strcmp(argv[1], argv[2])) {
        printf("Cannot convert to the same base\n");
        return 0;
    }


    int i = 0;
    if(!strcmp(argv[1], "bin")) {
        if(!strcmp(argv[2], "oct")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(bin_val(holder)) {
                    out_bin_oct(result, holder, 0);
                } else {
                    printf("NVbin");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "dec")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(bin_val(holder)) {
                    out_bin_dec(result, holder, 0);
                } else {
                    printf("NVbin");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "hex")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(bin_val(holder)) {
                    out_bin_hex(result, holder, 0);
                } else {
                    printf("NVbin");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "all")) {
            while(i < argc - 3) {
                printf("\n");
                dyn_str_copy(holder, argv[3 + i], 0);
                if(bin_val(holder)) {
                    dyn_str_print(holder, 0);
                    printf("_(2)\n");
                    out_bin_oct(result, holder, 0);
                    printf("_(8)\n");
                    out_bin_dec(result, holder, 0);
                    printf("_(10)\n");
                    out_bin_hex(result, holder, 0);
                    printf("_(16)\n");
                } else {
                    printf("NVbin\n");
                }
                i++;
            }
        }        
    }


    if(!strcmp(argv[1], "oct")) {
        if(!strcmp(argv[2], "bin")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(oct_val(holder)) {
                    out_oct_bin(result, holder, 0);
                } else {
                    printf("NVoct");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "dec")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(oct_val(holder)) {
                    out_oct_dec(result, holder, 0);
                } else {
                    printf("NVoct");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "hex")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(oct_val(holder)) {
                    out_oct_hex(result, holder, 0);
                } else {
                    printf("NVoct");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "all")) {
            while(i < argc - 3) {
                printf("\n");
                dyn_str_copy(holder, argv[3 + i], 0);
                if(oct_val(holder)) {
                    out_oct_bin(result, holder, 0);
                    printf("_(2)\n");
                    dyn_str_print(holder, 0);
                    printf("_(8)\n");
                    out_oct_dec(result, holder, 0);
                    printf("_(10)\n");
                    out_oct_hex(result, holder, 0);
                    printf("_(16)\n");
                } else {
                    printf("NVoct\n");
                }
                i++;
            }
        }
    }


    if(!strcmp(argv[1], "dec")) {
        if(!strcmp(argv[2], "bin")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(dec_val(holder)) {
                    out_dec_bin(result, holder, 0);
                } else {
                    printf("NVdec");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "oct")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(dec_val(holder)) {
                    out_dec_oct(result, holder, 0);
                } else {
                    printf("NVdec");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "hex")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                if(dec_val(holder)) {
                    out_dec_hex(result, holder, 0);
                } else {
                    printf("NVdec");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "all")) {
            while(i < argc - 3) {
                printf("\n");
                dyn_str_copy(holder, argv[3 + i], 0);
                if(dec_val(holder)) {
                    out_dec_bin(result, holder, 0);
                    printf("_(2)\n");
                    out_dec_oct(result, holder, 0);
                    printf("_(8)\n");
                    dyn_str_print(holder, 0);
                    printf("_(10)\n");
                    out_dec_hex(result, holder, 0);
                    printf("_(16)\n");
                } else {
                    printf("NVdec\n");
                }
                i++;
            }
        }
    
    }


    if(!strcmp(argv[1], "hex")) {
        if(!strcmp(argv[2], "bin")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                removecaps(holder);
                if(hex_val(holder)) {
                    out_hex_bin(result, holder, 0);
                } else {
                    printf("NVhex");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "oct")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                removecaps(holder);
                if(hex_val(holder)) {
                    out_hex_oct(result, holder, 0);
                } else {
                    printf("NVhex");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "dec")) {
            while(i < argc - 3) {
                dyn_str_copy(holder, argv[3 + i], 0);
                removecaps(holder);
                if(hex_val(holder)) {
                    out_hex_dec(result, holder, 0);
                } else {
                    printf("NVhex");
                }
                printf(" ");
                i++;
            }
        }
        if(!strcmp(argv[2], "all")) {
            while(i < argc - 3) {
                printf("\n");
                dyn_str_copy(holder, argv[3 + i], 0);
                removecaps(holder);
                if(hex_val(holder)) {
                    out_hex_bin(result, holder, 0);
                    printf("_(2)\n");
                    out_hex_oct(result, holder, 0);
                    printf("_(8)\n");
                    out_hex_dec(result, holder, 0);
                    printf("_(10)\n");
                    dyn_str_print(holder, 0);
                    printf("_(16)\n");
                } else {
                    printf("NVhex\n");
                }
                i++;
            }
        }
    }


    
    
    
    dyn_str_free(holder);
    dyn_str_free(*result);
    free(result);
    return 0;
}