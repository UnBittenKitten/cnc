// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h> 
// #include <math.h>
// //#include <Windows.h>
// //Future functionality
// //Floating point numbers
// //Transform between the bases (2 - 36)
// //If posible add arbitrary big numbers
//#include "libs/dynStr.h"
// #include "libs/gmp.h"
// #include "libs/limits.h"
// #include "libs/stddef.h"
// #include "libs/syslimits.h" //remove libraries and main


int maxvalue(int org, int max) {
    if(org > max) {
        return max;
    }
    return org;
}

int isInt(char str[]) {
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] < 48 || str[i] > 57) return 0;
    }
    return 1;
}

int ret_color_value(char color[]) {
    if(!strcmp(color, "darkblue")) return 1;
    if(!strcmp(color, "green")) return 2;
    if(!strcmp(color, "lightblue")) return 3;
    if(!strcmp(color, "red")) return 4;
    if(!strcmp(color, "purple")) return 5;
    if(!strcmp(color, "darkyellow")) return 6;
    if(!strcmp(color, "white")) return 7;
    if(!strcmp(color, "gray")) return 8;
    if(!strcmp(color, "blue")) return 9;
    if(!strcmp(color, "lightgreen")) return 10;
    if(!strcmp(color, "lightblue")) return 11;
    if(!strcmp(color, "orange")) return 12;
    if(!strcmp(color, "magenta")) return 13;
    if(!strcmp(color, "yellow")) return 14;
    return 0;
}

int num_valid(dynStr* dyn_str, int base) {
    int i = 0;
    int dotcheck = 0;
    while(i < strlen(*(dyn_str->str))) {
        if(*(*(dyn_str->str) + i) == '.') {
            dotcheck++;
            i++;
            continue;
        }
        if(*(*(dyn_str->str) + i) < 48 || *(*(dyn_str->str) + i) >= 48 + base) {
            return 0;
        }
        i++;
    }
    if(dotcheck > 1) return 0;
    return 1;
}

int low_case_valid(dynStr* dyn_str, int base) {
    int i = 0;
    char hld;
    int check = 0;
    int dotcheck = 0;
    while(i < strlen(*(dyn_str->str))) {
        if(*(*(dyn_str->str) + i) == '.') {
            dotcheck++;
            i++;
            continue;
        }
        check = 0;
        hld = *(*(dyn_str->str) + i);
        if(hld < 48 || hld >= 48 + maxvalue(base, 10)) { //base should be max 10
            check++;
        }   

        if(hld < 97 || hld >= 87 + maxvalue(base, 36)) {
            check++;
        }

        if(check >= 2) {
            return 0;
        }

        i++;
    }

    if(dotcheck > 1) return 0;
    return 1;
}

int upp_case_valid(dynStr* dyn_str, int base) {
    int i = 0;
    char hld;
    int check;
    int dotcheck = 0;

    while(i < strlen(*(dyn_str->str))) {
        if(*(*(dyn_str->str) + i) == '.') {
            dotcheck++;
            i++;
            continue;
        }
        check = 0;
        hld = *(*(dyn_str->str) + i);

        if(hld < 48 || hld >= 48 + maxvalue(base, 10)) {
            check++;
        }   

        if(hld < 97 || hld >= 87 + maxvalue(base, 36)) {
            check++;
        }

        if(hld < 65 || hld >= 29 + maxvalue(base, 62)) {
            check++;
        }

        if(check >= 3) {
            return 0;
        }
        i++;
    }
    if(dotcheck > 1) return 0;
    return 1;
}

int sexha_valid(dynStr* dyn_str, int base) {
    int i = 0;
    char hld;
    int check;
    int dotcheck = 0;

    while(i < strlen(*(dyn_str->str))) {
        if(*(*(dyn_str->str) + i) == '.') {
            dotcheck++;
            i++;
            continue;
        }
        check = 0;
        hld = *(*(dyn_str->str) + i);

        if(hld < 48 || hld >= 48 + maxvalue(base, 10)) {
            check++;
        }   

        if(hld < 97 || hld >= 87 + maxvalue(base, 36)) {
            check++;
        }

        if(hld < 65 || hld >= 29 + maxvalue(base, 62)) {
            check++;
        }

        if(hld != '+' && maxvalue(base, 63) == 63) {
            check++;
        }
        if(hld != '/' && maxvalue(base, 64) == 64) {
            check++;
        }

        if(base == 63) {
            if(check >= 4) {
                return 0;
            }
        }

        if(base == 64) {
            if(check >= 5) {
                return 0;
            }
        }
        i++;

    }
    if(dotcheck > 1) return 0;
    return 1;
}

int decider_validation(dynStr* dyn_str, int base) {
    if(base < 2 || base > 64) {
        printf("Invalid base\n");
        return 0;
    }

    if(base <= 10) {
        return num_valid(dyn_str, base);
    }
    if(base <= 36) {
        return low_case_valid(dyn_str, base);
    }
    if(base <= 62) {
        return upp_case_valid(dyn_str, base);
    }
    if(base <= 64) {
        return sexha_valid(dyn_str, base);
    }

    return 0;

}
