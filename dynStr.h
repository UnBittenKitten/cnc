#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20

typedef struct {
    char** str;
    int* lfe; //last free element
    int* limit;
} dynStr;

dynStr* dyn_str_init() {
    dynStr* new_str = (dynStr*)malloc(sizeof(dynStr));
    new_str->str = (char**)malloc(sizeof(char*));
    *(new_str->str) = (char*)malloc(sizeof(char) * MAX_SIZE);
    new_str->lfe = (int*)malloc(sizeof(int));
    new_str->limit = (int*)malloc(sizeof(int));

    *(new_str->lfe) = 0;
    *(new_str->limit) = MAX_SIZE;
    return new_str;
}

void dyn_str_free(dynStr* dyn_str) {
    free(*(dyn_str->str));
    free(dyn_str->str);
    free(dyn_str->lfe);
    free(dyn_str->limit);
    free(dyn_str);

}

void dyn_str_add(dynStr* dyn_str, char conStr[]) {
    //increase conditions
    if(strlen(conStr) + 1 > *(dyn_str->limit) - *(dyn_str->lfe) - 1) {
        //printf("INCREASING SIZE\n"); //DEBUG
        *(dyn_str->limit) = *(dyn_str->limit) * 2;
        *(dyn_str->str) = (char*)realloc(*(dyn_str->str), *(dyn_str->limit));
    }

    int orgSize = strlen(conStr);
    int i = 0;
    while(i < orgSize) {
        //printf("adding %c\n", conStr[i]); //DEBUG
        *(*(dyn_str->str) + *(dyn_str->lfe)) = conStr[i];
        *(dyn_str->lfe) = *(dyn_str->lfe) + 1;
        i++;
    }
    *(*(dyn_str->str) + *(dyn_str->lfe)) = 0;

}

void dyn_str_print(dynStr* dyn_str, int printNewLine) {
    printf("%s", *(dyn_str->str));
    if(printNewLine) {
        printf("\n");
    }
}

void dyn_str_del(dynStr* dyn_str) {
    free(*dyn_str->str);
    *(dyn_str->lfe) = 0;
    *(dyn_str->limit) = MAX_SIZE;
}

// void dyn_str_copy(dynStr* dest, char src[]) {
    
// }