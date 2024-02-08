//Version 2.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <Windows.h>
#include "libs/dynStr.h"
#include "libs/gmp.h"
#include "libs/limits.h"
#include "libs/stddef.h"
#include "libs/syslimits.h"
#include "libs/cJSON.c"
#include "libs/DStack.c"
#include "validate.h"
#include "arithmetic.h"

char* colors[] = {"darkblue", "green", "lightblue", "red", "purple", "darkyellow", "white", "gray", "blue", "lightgreen", "lightblue", "orange", "magenta", "yellow", 0};

int strval1(char str[]) {
    if(!strcmp(str, "bin")) return 2;
    if(!strcmp(str, "oct")) return 8;
    if(!strcmp(str, "dec")) return 10;
    if(!strcmp(str, "hex")) return 16;

    if(!isInt(str)) return 0;
    if(strlen(str) > 2) return 0;
    int base = atoi(str);
    if(base < 2 || base > 64) return 0;
    return base;


    return 0;
}

int strval2(char str[]) {
    if(!strcmp(str, "bin")) return 2;
    if(!strcmp(str, "oct")) return 8;
    if(!strcmp(str, "dec")) return 10;
    if(!strcmp(str, "hex")) return 16;
    if(!strcmp(str, "all")) return -1;

    if(!isInt(str)) return 0;
    if(strlen(str) > 2) return 0;
    int base = atoi(str);
    if(base < 2 || base > 64) return 0;
    return base;


    return 0;
}

void helpcmd(int argc, char* argv[]) {

    if(argc == 2) {
        printf("cnc is a C based console application designed to easily transform between numeric bases.\n");
        printf("To use it type \"cnc\" followed by two numbers, the first number you input is going to be\n");
        printf("the base you transform from, and the second is the base you will transform to.\n");
        printf("Afterwards, type all the numbers you want to transform, remembering the valid characters for each base.\n");
        printf("If you want to see the characters for a specific base type \"cnc -c available-characters n\", n being the \n");
        printf("base you want to know the characters of.\n");
        printf("You can also type \"bin\", \"oct\", \"dec\" and \"hex\" instead of 2, 8, 10 and 16 respectively\n");
        printf("Aditionally you can type \"all\" as the second base, and the program will transform the number(s)\n");
        printf("you input into all the bases specified in the config file (default: 2, 8, 10, 16).\n");
        printf("If you want more information about printing multiple bases at the same time type \"cnc -h -c print-all\"\n");
        printf("\n");
        printf("You can know more about the configurations by typing cnc -h -c\n");
        return;
    }

    if(!strcmp(argv[2], "-c") || !strcmp(argv[2], "config")) {
        printf("The available configurations are:\n");
        printf("\tprimary-color, -pc:      changes the first color used when printing numbers\n");
        printf("\tsecondary-color, -sc:    changes the second color used when printing numbers\n");
        printf("To see the available colors type \"cnc -c valid-colors\"\n");
        printf("\tprecision                changes how many digits are printed when transforming floating point numbers\n");
        printf("The following are print-all commands, and must be typed like this cnc -c print-all \"command\"\n");
        printf("\tprint-input-base, -pib:  changes if the base you input is printed when using \"all\"\n");
        printf("\tadd                      adds a numeric base to the bases printed with \"all\"\n");
        printf("\tdelete, del              delets a numeric base to the bases printed with \"all\"\n");
        printf("\tquery                    returns the bases printed with \"all\"\n");
        printf("\treset, -r                resets the bases printed with \"all\"\n");
        // printf("\n");

        return;
    }
    


    return;
}

int exists(char fname[]) {
    FILE *fp;
    if((fp = fopen(fname, "r"))) {
        fclose(fp);
        return 1;
    } else {
        return 0;
    }
}

int readjson(cJSON** read_cjson, char filename[]) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error: Unable to open the file\n");
        return 0;
    }   

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp) + 20;
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(size * sizeof(char));
    fread(buffer, 1, size, fp);
    fclose(fp);

    *read_cjson = cJSON_Parse(buffer);
    if(*read_cjson == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(*read_cjson);
        return 0;
    }
    free(buffer);

    return 1;
}

int generate_config() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "primary-color", "white");
    cJSON_AddStringToObject(json, "secondary-color", "lightblue");
    cJSON_AddNumberToObject(json, "precision", 10);
    cJSON_AddBoolToObject(json, "print-input-base", 1);

    cJSON *bases = cJSON_AddArrayToObject(json, "bases");
    cJSON_AddItemToArray(bases, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(bases, cJSON_CreateNumber(8));
    cJSON_AddItemToArray(bases, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(bases, cJSON_CreateNumber(16));

    char *json_str = cJSON_Print(json);

    FILE *fp = fopen("config.json", "w");
    if(fp == NULL) {
        printf("Error: Unable to create config file\n");
        return 1;
    }
    fputs(json_str, fp);
    fclose(fp);

    cJSON_free(json_str);
    cJSON_Delete(json);
    //cJSON_Delete(bases);
    printf("Generated config file\n");
    return 0;
}

void config(int argc, char* argv[]) {
    cJSON* json;
    readjson(&json, "config.json");
    if(!json) {
        printf("Error: Unable to open config file\n");
        return;
    }
    if(!strcmp(argv[2], "print-all")) { //usr
        if(argc == 3) {
            printf("Not enough arguments\n");
            return;
        }
        if(!strcmp(argv[3], "query")) {
            if(argc > 4) {
                printf("Too many arguments\n");
                cJSON_Delete(json);
                return;
            }
            cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, "bases");
            if(!arr) {
                printf("Unable to get \"bases\" from config file\n");
                cJSON_Delete(json);
                return;
            }

            cJSON* iterator = NULL;
            printf("Bases: [");
            cJSON_ArrayForEach(iterator, arr) {
                if(cJSON_IsNumber(iterator)) {
                    printf("%0.lf", iterator->valuedouble);
                    if(iterator->next) printf(" ");
                } else {
                    printf("invalid");
                }
            }
            printf("]\n");
            // cJSON_Delete(arr);
            // cJSON_Delete(iterator);

            cJSON_Delete(json);
            return;
        }

        if(!strcmp(argv[3], "add")) {
            if(argc > 5) {
                printf("Too many arguments (Can only input one base at a time)\n");
                cJSON_Delete(json);
                return;
            }
            if(!isInt(argv[4])) {
                printf("\"%s\" is not a valid base.\n", argv[4]);
                cJSON_Delete(json);
                return;
            }


            //printf("Adding base \"%s\"\n", argv[4]);
            char* ptr;
            int base = strtol(argv[4], &ptr, 10);
            if(base < 2 || base > 64) {
                printf("\"%s\" is not a valid base.\n", argv[4]);
                cJSON_Delete(json);
                return;
            }

            cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, "bases");
            if(!arr) {
                printf("Unable to get \"bases\" from config file\n");
                cJSON_Delete(json);
                return;
            }
            cJSON* iterator = NULL;
            DStack* stack = dstack_init();


            cJSON_ArrayForEach(iterator, arr) {
                if(cJSON_IsNumber(iterator)) {
                    dstack_push(stack, iterator->valuedouble);
                } else {
                    printf("invalid");
                    cJSON_Delete(json);
                    return;
                }
            }
            for(int i = 0; i < stack->elem; i++) {
                if((stack->arr)[i] == base) {
                    printf("\"%s\" is a repeated base\n", argv[4]);
                    cJSON_Delete(json);
                    dstack_free(stack);
                    return;
                }
            }
            dstack_push(stack, base);
            dstack_sort(stack);


            cJSON* new = cJSON_CreateArray();
            for(int i = 0; i < stack->elem; i++) {
                cJSON_AddItemToArray(new, cJSON_CreateNumber((stack->arr)[i]));
            }
            cJSON_ReplaceItemInObjectCaseSensitive(json, "bases", new);
            char* json_str = cJSON_Print(json);

            FILE* fp = fopen("config.json", "w");
            if(fp == NULL) {
                printf("Error: Unable to open file \"config.json\"\n");
                cJSON_Delete(json);
                return;
            }
            fputs(json_str, fp);
            fclose(fp);


            arr = cJSON_GetObjectItemCaseSensitive(json, "bases");
            if(!arr) {
                printf("Unable to get \"bases\" from config file\n");
                cJSON_Delete(json);
                return;
            }
            iterator = NULL;

            printf("Bases: [");
            cJSON_ArrayForEach(iterator, arr) {
                if(cJSON_IsNumber(iterator)) {
                    printf("%0.lf", iterator->valuedouble);
                    if(iterator->next) printf(" ");
                } else {
                    printf("invalid");
                    cJSON_Delete(json);
                    return;
                }
            }
            printf("]\n");

            
            cJSON_free(json_str);
            dstack_free(stack);
            cJSON_Delete(json);
            return;
        }

        if(!strcmp(argv[3], "del") || !strcmp(argv[3], "delete")) {
            if(argc > 5) {
                printf("Too many arguments (Can only input one base at a time)\n");
                cJSON_Delete(json);
                return;
            }
            if(!strcmp(argv[4], "all")) {
                char input[10];
                printf("Are you sure you want to delete all the bases? (Y/n) ");
                fgets(input, 5, stdin);
                //printf("input = %s\n", input);
                if(!strcmp(input, "n\n") || !strcmp(input, "N\n")) {
                    printf("...\n");
                    cJSON_Delete(json);
                    return;
                } else {
                    if((strcmp(input, "y\n") && strcmp(input, "Y\n"))) {
                        printf("Only valid inputs are \"y\" and \"n\"\n");
                        cJSON_Delete(json);
                        return;
                    }
                }
                cJSON* new = cJSON_CreateArray();
                // cJSON_AddItemToArray(new, cJSON_CreateNumber(2));
                // cJSON_AddItemToArray(new, cJSON_CreateNumber(8));
                // cJSON_AddItemToArray(new, cJSON_CreateNumber(10));
                // cJSON_AddItemToArray(new, cJSON_CreateNumber(16));
                cJSON_ReplaceItemInObjectCaseSensitive(json, "bases", new);
                char* json_str = cJSON_Print(json);

                FILE* fp = fopen("config.json", "w");
                if(fp == NULL) {
                    printf("Error: Unable to open file \"config.json\"\n");
                    cJSON_Delete(json);
                    return;
                }
                fputs(json_str, fp);
                fclose(fp);
                cJSON_free(json_str);
                cJSON_Delete(json);
                return;
            }
            if(!isInt(argv[4])) {
                printf("\"%s\" is not a valid base.\n", argv[4]);
                cJSON_Delete(json);
                return;
            }
            
            char* ptr;
            int base = strtol(argv[4], &ptr, 10);
            if(base < 2 || base > 64) {
                printf("\"%s\" is not a valid base.\n", argv[4]);
                cJSON_Delete(json);
                return;
            }

            cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, "bases");
            if(!arr) {
                printf("Unable to get \"bases\" from config file\n");
                cJSON_Delete(json);
                return;
            }
            cJSON* iterator = NULL;
            DStack* stack = dstack_init();
            int j;

            cJSON_ArrayForEach(iterator, arr) {
                if(cJSON_IsNumber(iterator)) {
                    dstack_push(stack, iterator->valuedouble);
                } else {
                    printf("invalid");
                    cJSON_Delete(json);
                    return;
                }
            }

            for(int i = 0; i < stack->elem; i++) {
                if((stack->arr)[i] == base) {
                    j = 1;
                }
            }

            if(!j) {
                printf("\"%s\" is not an active base\n", argv[4]);
                cJSON_Delete(json);
                dstack_free(stack);
                return;
            }

            dstack_searchdel(stack, base);
            dstack_sort(stack); //optional

            cJSON* new = cJSON_CreateArray();
            for(int i = 0; i < stack->elem; i++) {
                cJSON_AddItemToArray(new, cJSON_CreateNumber((stack->arr)[i]));
            }
            cJSON_ReplaceItemInObjectCaseSensitive(json, "bases", new);
            char* json_str = cJSON_Print(json);

            FILE* fp = fopen("config.json", "w");
            if(fp == NULL) {
                printf("Error: Unable to open file \"config.json\"\n");
                cJSON_Delete(json);
                return;
            }
            fputs(json_str, fp);
            fclose(fp);


            arr = cJSON_GetObjectItemCaseSensitive(json, "bases");
            if(!arr) {
                printf("Unable to get \"bases\" from config file\n");
                cJSON_Delete(json);
                return;
            }
            iterator = NULL;

            printf("Bases: [");
            cJSON_ArrayForEach(iterator, arr) {
                if(cJSON_IsNumber(iterator)) {
                    printf("%0.lf", iterator->valuedouble);
                    if(iterator->next) printf(" ");
                } else {
                    printf("invalid");
                    cJSON_Delete(json);
                    return;
                }
            }
            printf("]\n");

            
            cJSON_free(json_str);
            dstack_free(stack);
            cJSON_Delete(json);
            return;
        }

        if(!strcmp(argv[3], "print-input-base") || !strcmp(argv[3], "-pib")) {
            if(argc < 5) {
                printf("Too few arguments\n");
                cJSON_Delete(json);
                return;
            }
            if(argc > 5) {
                printf("Too many arguments\n");
                cJSON_Delete(json);
                return;
            }
            if(strcmp(argv[4], "1") && strcmp(argv[4], "true") && strcmp(argv[4], "0") && strcmp(argv[4], "false")) {
                printf("Invalid input, only available inputs are \"1\", \"0\", \"true\" and \"false\"\n");
                cJSON_Delete(json);
                return;
            }

            cJSON* a = cJSON_GetObjectItemCaseSensitive(json, "print-input-base");

            if(!a) {
                printf("Error: Unable to open \"print-input-base\"\n");
                cJSON_Delete(json);
                return;
            }

            if(!strcmp(argv[4], "1") || !strcmp(argv[4], "true")) {
                cJSON_ReplaceItemInObjectCaseSensitive(json, "print-input-base", cJSON_CreateTrue());
                printf("Set \"print-input-base\" to true\n");
            }

            if(!strcmp(argv[4], "0") || !strcmp(argv[4], "false")) {
                cJSON_ReplaceItemInObjectCaseSensitive(json, "print-input-base", cJSON_CreateFalse());
                printf("Set \"print-input-base\" to false\n");
            }

            char* json_str = cJSON_Print(json);

            FILE* fp = fopen("config.json", "w");
            if(fp == NULL) {
                printf("Error: Unable to open file \"config.json\"\n");
                cJSON_Delete(json);
                return;
            }
            fputs(json_str, fp);
            fclose(fp);

            cJSON_free(json_str);

            cJSON_Delete(json);
            return;
        }

        if(!strcmp(argv[3], "reset") || !strcmp(argv[3], "-r")) {
            char input[10];
            printf("Are you sure you want to reset the config base? (Y/n) ");
            fgets(input, 5, stdin);
            //printf("input = %s\n", input);
            if(!strcmp(input, "n\n") || !strcmp(input, "N\n")) {
                printf("...\n");
                cJSON_Delete(json);
                return;
            } else {
                if((strcmp(input, "y\n") && strcmp(input, "Y\n"))) {
                    printf("Only valid inputs are \"y\" and \"n\"\n");
                    cJSON_Delete(json);
                    return;
                }
            }
            cJSON* new = cJSON_CreateArray();
            cJSON_AddItemToArray(new, cJSON_CreateNumber(2));
            cJSON_AddItemToArray(new, cJSON_CreateNumber(8));
            cJSON_AddItemToArray(new, cJSON_CreateNumber(10));
            cJSON_AddItemToArray(new, cJSON_CreateNumber(16));
            cJSON_ReplaceItemInObjectCaseSensitive(json, "bases", new);
            char* json_str = cJSON_Print(json);
            FILE* fp = fopen("config.json", "w");
            if(fp == NULL) {
                printf("Error: Unable to open file \"config.json\"\n");
                cJSON_Delete(json);
                return;
            }
            fputs(json_str, fp);
            cJSON_free(json_str);
            fclose(fp);

            cJSON* arr = cJSON_GetObjectItemCaseSensitive(json, "bases");
            if(!arr) {
                printf("Unable to get \"bases\" from config file\n");
                cJSON_Delete(json);
                return;
            }
            cJSON* iterator = NULL;

            printf("Bases: [");
            cJSON_ArrayForEach(iterator, arr) {
                if(cJSON_IsNumber(iterator)) {
                    printf("%0.lf", iterator->valuedouble);
                    if(iterator->next) printf(" ");
                } else {
                    printf("invalid");
                    cJSON_Delete(json);
                    return;
                }
            }
            printf("]\n");
            cJSON_Delete(json);
            return;
            
        }
        
        printf("\"%s\" is not a valid configuration argument\n", argv[3]);
        cJSON_Delete(json);
        return;
    }

    if(!strcmp(argv[2], "valid-colors")) {
        int i = 0;
        while(colors[i] != 0) {
            printf("%s", colors[i++]);
            if(colors[i] != 0) printf(", ");
        }
        printf("\n");
        return;
    }

    if((!strcmp(argv[2], "primary-color")) || (!strcmp(argv[2], "-pc"))) {
        if(argc > 4) {
            printf("Too many arguments\n");
            cJSON_Delete(json);
            return;
        }

        if(!ret_color_value(argv[3])) {
            printf("\"%s\" is not a valid color\n", argv[3]);
            printf("For list of available colors type \"cnc -c valid-colors\"\n");
            cJSON_Delete(json);
            return;
        }

        cJSON* color = cJSON_GetObjectItemCaseSensitive(json, "primary-color");
        if(!color) {
            printf("Error: Unable to get \"primary-color\" from config file\n");
            cJSON_Delete(json);
            return;
        }
        
        cJSON_ReplaceItemInObjectCaseSensitive(json, "primary-color", cJSON_CreateString(argv[3]));
        char* json_str = cJSON_Print(json);

        FILE* fp = fopen("config.json", "w");
        if(fp == NULL) {
            printf("Error: Unable to open file \"config.json\"\n");
            cJSON_Delete(json);
            return;
        }
        fputs(json_str, fp);
        fclose(fp);
        printf("Set primary color to %s.", argv[3]); //optional
        cJSON_free(json_str);
        cJSON_Delete(json);
        return;
    }

    if((!strcmp(argv[2], "secondary-color")) || (!strcmp(argv[2], "-sc"))) {
        if(argc > 4) {
            printf("Too many arguments\n");
            cJSON_Delete(json);
            return;
        }

        if(!ret_color_value(argv[3])) {
            printf("\"%s\" is not a valid color\n", argv[3]);
            printf("For list of available colors type \"cnc -c valid-colors\"\n");
            cJSON_Delete(json);
            return;
        }

        cJSON* color = cJSON_GetObjectItemCaseSensitive(json, "secondary-color");
        if(!color) {
            printf("Error: Unable to get \"secondary-color\" from config file\n");
            cJSON_Delete(json);
            return;
        }

        cJSON_ReplaceItemInObjectCaseSensitive(json, "secondary-color", cJSON_CreateString(argv[3]));
        char* json_str = cJSON_Print(json);

        FILE* fp = fopen("config.json", "w");
        if(fp == NULL) {
            printf("Error: Unable to open file \"config.json\"\n");
            cJSON_Delete(json);
            return;
        }
        fputs(json_str, fp);
        fclose(fp);
        printf("Set secondary color to %s.", argv[3]); //optional
        cJSON_free(json_str);
        cJSON_Delete(json);
        return;
    }

    if(!strcmp(argv[2], "reset") || !strcmp(argv[2], "-r")) {
        char input[10];
        printf("Are you sure you want to reset the bases? (Y/n) ");
        fgets(input, 5, stdin);
        //printf("input = %s\n", input);
        if(!strcmp(input, "n\n") || !strcmp(input, "N\n")) {
            printf("...\n");
            cJSON_Delete(json);
            return;
        } else {
            if((strcmp(input, "y\n") && strcmp(input, "Y\n"))) {
                printf("Only valid inputs are \"y\" and \"n\"\n");
                cJSON_Delete(json);
                return;
            }
        }

        generate_config();
        return;
    }

    if(!strcmp(argv[2], "precision")) {
        if(argc < 4) {
            printf("Too few arguments\n");
            cJSON_Delete(json);
            return;
        }
        if(argc > 4) {
            printf("Too many arguments\n");
            cJSON_Delete(json);
            return;
        }

        char* ptr;
        int pre = strtol(argv[3], &ptr, 10);
        if(!isInt(argv[3]) || pre < 1 || pre >= 100) {
            printf("\"%s\" is not a valid number for precision\n", argv[3]);
            printf("Must be a number between 1 and 100\n");
        }

        cJSON* jspre = cJSON_GetObjectItemCaseSensitive(json, "precision");
        if(!jspre) {
            printf("Error: Unable to get \"precision\" from config file\n");
            cJSON_Delete(json);
            return;
        }

        cJSON_ReplaceItemInObjectCaseSensitive(json, "precision", cJSON_CreateNumber(pre));

        char* json_str = cJSON_Print(json);

        FILE* fp = fopen("config.json", "w");
        if(fp == NULL) {
            printf("Error: Unable to open file \"config.json\"\n");
            cJSON_Delete(json);
            return;
        }
        fputs(json_str, fp);
        fclose(fp);
        printf("Set precision to %s.", argv[3]); //optional

        cJSON_free(json_str);
        cJSON_Delete(json);
        return;
    }

    if(!strcmp(argv[2], "available-characters")) {
        int j;
        if(!(j = isInt(argv[3])) || strlen(argv[3]) > 2) {
            printf("\"%s\" is not a valid numeric base", argv[3]);
            return;
        }

        j = atoi(argv[3]);
        
        if(j == 1) {
            printf("1 is not a valid numeric base\n");
            return;
        }

        if(j > 64) {
            printf("64 is the maximum numeric base support\n");
            return;
        }

        // printf("Capitals letter are not accepted as lower case letters and viceversa\n");
        printf("Character | Value (decimal)\n");
        for(int k = 0; k < j; k++) {
            printf("%c = %d\n", returnchar(k), k);
        }

        return;
    }    

    printf("\"%s\" is not a valid configuration argument\n", argv[2]);
}

int main(int argc, char* argv[]) {

    if(!exists("config.json")) {
        if(generate_config()) {
            printf("Error: Unable to generate config file\n");
        }
    }

    if(argc == 1) {
        printf("Type \"cnc -h\" for help\n");
        return 0;
    }

    if((!strcmp(argv[1], "-h") || !strcmp(argv[1], "help"))) {
        helpcmd(argc, argv);
        return 0;
    }

    if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "config")) {
        if(argc == 2) {
            printf("You need to input more arguments\n");
            printf("If you need help type \"cnc -h -c\"\n");
            return 0;
        }
        config(argc, argv);
        return 0;
    }

    int bases[2];

    
    if(!(bases[0] = strval1(argv[1]))) {
        printf("Invalid first argument, \"%s\" is not a valid base\n", argv[1]);
        printf("For help type \"cnc -h\"\n");
        return 0;
    }

    if(!(bases[1] = strval2(argv[2]))) {
        printf("Invalid second argument, \"%s\" is not a valid base\n", argv[2]);
        printf("For help type \"cnc -h\"\n");
        return 0;
    }

    // if(bases[0] == bases[1]) {
    //     printf("Can not convert to the same base\n");
    //     return 0;
    // }

    if(argc < 4) {
        printf("Not enough arguments\n");
        return 0;
    }

    dynStr* holder1 = dyn_str_init();
    dynStr* holder2 = dyn_str_init();
    dynStr** result = (dynStr**)malloc(sizeof(dynStr*));
    *result = dyn_str_init();
    mpz_t* arr = malloc(4 * sizeof(mpz_t));
    for(int i = 0; i < 4; i++) mpz_init(*(arr + i)); 

    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    
    cJSON* root;
    readjson(&root, "config.json");
    if(!root) {
        printf("Unable to open config file\n");
        goto eop;
    }
    cJSON* prmcol = cJSON_GetObjectItemCaseSensitive(root, "primary-color");
    if(!prmcol) {
        printf("Unable to get \"primary-color\" from config file\n");
        goto eop;
    }
    cJSON* scncol = cJSON_GetObjectItemCaseSensitive(root, "secondary-color");
    if(!scncol) {
        printf("Unable to get \"secondary-color\" from config file\n");
        goto eop;
    }
    cJSON* pre = cJSON_GetObjectItemCaseSensitive(root, "precision");
    if(!pre) {
        printf("Unable to get \"precision\" from config file\n");
        goto eop;
    }
    int precision = pre->valuedouble;
    int j = 0;
    int i = 0;
    int dflCol = ret_color_value(prmcol->valuestring);
    int sndCol = ret_color_value(scncol->valuestring);

    if(bases[1] == -1) {
        cJSON* arrjson = cJSON_GetObjectItemCaseSensitive(root, "bases");
        cJSON* iterator = NULL;
        DStack* stack = dstack_init();


        cJSON_ArrayForEach(iterator, arrjson) {
            if(cJSON_IsNumber(iterator)) {
                dstack_push(stack, iterator->valuedouble);
            } else {
                printf("invalid");
                dstack_free(stack);
                goto eop;
                return 0;
            }
        }

        if(stack->elem == 0) {
            printf("No bases are configured to print\n");
            dstack_free(stack);
            goto eop;
        }

        cJSON* inpBase = cJSON_GetObjectItemCaseSensitive(root, "print-input-base");
        int printbase = inpBase->valueint;

        if(printbase) {
            for(int i = 0; i < stack->elem; i++) 
                if((stack->arr)[i] == bases[0]) printbase = 0;   
        }
        if(printbase) {
            dstack_push(stack, bases[0]);
            dstack_sort(stack);
        }

        while(i < (argc - 3)) {
            dyn_str_copy(holder1, argv[3 + i], 0);
            if(decider_validation(holder1, bases[0])) {
                if(isZeros(holder1)) {
                    printf("0\n");
                } else {
                    for(int k = 0; k < stack->elem; k++) {
                        dyn_str_copy(holder1, argv[3 + i], 0);
                        out_any_any(holder1, holder2, *result, bases[0], (stack->arr)[k], arr, precision, 0);
                        printf("_(%.0lf)\n", (stack->arr)[k]);
                    }
                }
            } else {
                printf("NotValid\n");
            }
            i++;
            if(i != argc - 3) {
                printf("\n");
            }            
        }
        


        dstack_free(stack);
        goto eop;
    }

    while(i < (argc - 3)) {
        if(j % 2 == 0) {
            SetConsoleTextAttribute(hCon, dflCol);
        } else {
            SetConsoleTextAttribute(hCon, sndCol);
        }
        j++;
        dyn_str_copy(holder1, argv[3 + i], 0);

        if(decider_validation(holder1, bases[0])) {
            if(isZeros(holder1)) {
                printf("0");
            } else {
                out_any_any(holder1, holder2, *result, bases[0], bases[1], arr, precision, 0);
            }
        } else {
            printf("NotValid");
        }
        printf(" ");

        i++;
    }
    
    
    eop:
    SetConsoleTextAttribute(hCon, 7);
    for(int i = 0; i < 4; i++) mpz_clear(*(arr + i));
    cJSON_Delete(root);
    dyn_str_free(holder1);
    dyn_str_free(holder2);
    dyn_str_free(*result);
    free(result);
    return 0;
}