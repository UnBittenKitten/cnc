// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h> 
// #include <math.h>
// // #include <Windows.h>
// //Future functionality
// //Floating point numbers
// //Transform between the bases (2 - 36)
// //If posible add arbitrary big numbers
// #include "libs/dynStr.h"
// #include "libs/gmp.h"
// #include "libs/limits.h"
// #include "libs/stddef.h"
// #include "libs/syslimits.h"

int getvalue(char digit) {
    if(48 <= digit && digit <= 57) {
        return digit - 48;
    }
    if(97 <= digit && digit <= 122) {
        return digit - 87;
    }
    if(65 <= digit && digit <= 90) {
        return digit - 29;
    }
    if(digit == '+') {
        return 62;
    }
    if(digit == '/') {
        return 63;
    }
    return 0;
}

char returnchar(int num) {
    if(0 <= num && num <= 9) {
        return num + 48;
    }
    if(10 <= num && num <= 35) {
        return num + 87;
    }
    if(36 <= num && num <= 61) {
        return num + 29;
    }
    if(num == 62) {
        return '+';
    }
    if(num == 63) {
        return '/';
    }

    return 48;
}

double maxval(double org, double max) {
    if(org >= max) {
        return max;
    }
    return org;
}

void out_float_any(dynStr* dyn_str, 
                   dynStr* result, 
                   int baseIn, 
                   int baseOut, 
                   int precision,
                   int newline) {
    double base10 = 0;
    int i = 0;
    int j = -1;
    double temp;
    int integerpart;

    while(*(*dyn_str->str + i) != 0) {
        base10 += pow(baseIn, j) * getvalue(*(*dyn_str->str + i));
        i++;
        j--;
    }

    dyn_str_clear(result, 0);
    i = 0;
    while(i < precision) {
        temp = pow(baseOut, -(i + 1));
        if(base10 >= pow(baseOut, -(i + 1))) {
            dyn_str_addchar(result, returnchar(base10 / temp));
            integerpart = base10/temp;
            base10 = base10 - maxval(integerpart * temp, (baseOut - 1) * pow(baseOut, -(i + 1)));
        } else {
            dyn_str_addchar(result, '0');
        }

        i++;
    }
    dyn_str_print(result, newline);
    return;
}

void out_int_any(dynStr* dyn_str, 
                 dynStr* result, 
                 int baseIn, 
                 int baseOut,
                 mpz_t* arr, 
                 int newline) {
    int tempint;
    int i = 0;

    dyn_str_rev(dyn_str);
    
    while(*(*dyn_str->str + i) != 0) {
        mpz_ui_pow_ui(arr[1], baseIn, i);
        mpz_set_ui(arr[2], getvalue(*(*dyn_str->str + i)));
        mpz_mul(arr[3], arr[1], arr[2]);

        mpz_add(arr[0], arr[0], arr[3]);
        i++;
    }

    i = 0;
    dyn_str_clear(result, 0);
    while(mpz_cmp_d(arr[0], 0) && i < 1000000) { //change for future release
        mpz_mod_ui(arr[1], arr[0], baseOut);
        tempint = mpz_get_ui(arr[1]);
        dyn_str_addchar(result, returnchar(tempint));

        mpz_tdiv_q_ui(arr[0], arr[0], baseOut);
        i++;
    }


    dyn_str_rev(result);
    dyn_str_print(result, newline);
    return;
}

int hasdot(dynStr* dyn_str) {
    for(int i = 0; i < strlen(*dyn_str->str); i++) {
        if(*(*dyn_str->str + i) == '.') return i;
    }
    return -1;
}

void trunczeros(dynStr* dyn_str) {
    int i = 0;
    while(*(*dyn_str->str + i) == '0') {
        i++;
    }
    int j;
    for(j = 0; j < strlen(*dyn_str->str) - i; j++) {
        *(*dyn_str->str + j) = *(*dyn_str->str + j + i);
    }

    for(int k = strlen(*dyn_str->str); k >= j; k--) {
        *(*dyn_str->str + k) = 0;
    }

}

int isZeros(dynStr* dyn_str) {
    int i = 0;
    while(*(*dyn_str->str + i) != 0) {
        if(*(*dyn_str->str + i) != '0' && *(*dyn_str->str + i) != '.') return 0;
        i++;
    }
    return 1;
}

void out_any_any(dynStr* hold1,
                 dynStr* hold2,
                 dynStr* result, 
                 int baseIn, 
                 int baseOut, 
                 mpz_t* arr,
                 int precision,
                 int newline) {
    
    int dot;
    if((dot = hasdot(hold1)) < 0) {
        out_int_any(hold1, result, baseIn, baseOut, arr, newline);
        return;
    }

    if(dot != 0) {
    dyn_str_snip(hold2, hold1, 0, dot - 1, 0);
    
    out_int_any(hold2, result, baseIn, baseOut, arr, 0);
    printf(".");
    dyn_str_snip(hold2, hold1, dot + 1, strlen(*hold1->str), 0);
    out_float_any(hold2, result, baseIn, baseOut, precision, newline);
    return;
    }

    printf("0.");
    dyn_str_snip(hold2, hold1, 1, strlen(*hold1->str), 0);
    out_float_any(hold2, result, baseIn, baseOut, precision, newline);

}
