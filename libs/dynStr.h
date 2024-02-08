// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//version 1.2
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
    if(!dyn_str) return;
    if(!(dyn_str->str)) return;
    if(!(*dyn_str->str)) return;
    free(*(dyn_str->str));
    free(dyn_str->str);
    free(dyn_str->lfe);
    free(dyn_str->limit);
    free(dyn_str);

}

void dyn_str_add(dynStr* dyn_str, char conStr[]) {
    if(!dyn_str) return;
    if(!(dyn_str->str)) return;
    if(!(*dyn_str->str)) return;

    //can improve by only resizing once even if the limit is increased more than once
    while(strlen(conStr) + 1 > *(dyn_str->limit) - *(dyn_str->lfe) - 1) {
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
    if(!dyn_str) return;
    if(!(dyn_str->str)) return;
    if(!(*dyn_str->str)) return;
    printf("%s", *(dyn_str->str));
    if(printNewLine) {
        printf("\n");
    }
}

void dyn_str_rev(dynStr* dyn_str) {
    if(!dyn_str) return;
    if(!(dyn_str->str)) return;
    if(!(*dyn_str->str)) return;

    dynStr* temp = dyn_str_init();
    int i = strlen(*dyn_str->str);
    char holdstr[2];
    holdstr[1] = 0;
    while(i >= 0) {
        holdstr[0] = *(*(dyn_str->str) + i);
        dyn_str_add(temp, holdstr);

        i--;
    }    
    strcpy(*(dyn_str->str), *(temp->str));


    dyn_str_free(temp);
}

void dyn_str_addchar(dynStr* dyn_str, char chr) {
    if(!dyn_str) return;
    if(!(dyn_str->str)) return;
    if(!(*dyn_str->str)) return;

    if(2 > *(dyn_str->limit) - *(dyn_str->lfe) - 1) {
        //printf("INCREASING SIZE\n"); //DEBUG
        *(dyn_str->limit) = *(dyn_str->limit) * 2;
        *(dyn_str->str) = (char*)realloc(*(dyn_str->str), *(dyn_str->limit));
    }

    *((*dyn_str->str) + *(dyn_str->lfe)) = chr;
    *(dyn_str->lfe) = *(dyn_str->lfe) + 1;
    *(*(dyn_str->str) + *(dyn_str->lfe)) = 0;


}

void dyn_str_clear(dynStr* dyn_str, int resize) {
    if(!dyn_str) return;

    if(resize) {
        free(*(dyn_str->str));
        *(dyn_str->str) = (char*)malloc(sizeof(char) * MAX_SIZE);
        *(dyn_str->lfe) = 0;
        *(dyn_str->limit) = MAX_SIZE;
        return;
    }

    for(int i = 0; i < *dyn_str->lfe; i++) {
        //*(*(dyn_str->str) + i) = ' ';
        *(*(dyn_str->str) + i) = 0;
    }
    *(dyn_str->lfe) = 0;

}

void dyn_str_copy(dynStr* dest, char source[], int resize) {
    if(!dest) return;
    if(!(dest->str)) return;
    if(!(*dest->str)) return;

    // while(strlen(conStr) + 1 > *(dyn_str->limit) - *(dyn_str->lfe) - 1) {
    //     *(dyn_str->limit) = *(dyn_str->limit) * 2;
    //     *(dyn_str->str) = (char*)realloc(*(dyn_str->str), *(dyn_str->limit));
    // }
    
    dyn_str_clear(dest, resize);

    if(strlen(source) + 1 > *dest->limit - 1) {
        while(strlen(source) + 1 > *dest->limit - 1) {
            *(dest->limit) = *(dest->limit) * 2;
        }
        *(dest->str) = (char*)realloc(*(dest->str), *(dest->limit));
    }

    strcpy(*(dest->str), source);
    *(dest->lfe) = strlen(source);

}

void dyn_str_snip(dynStr* dest, dynStr* src, int startindex,int endindex, int resize) { //inclusive
    if(!src) return;
    if(!(src->str)) return;
    if(!(*src->str)) return;
    if(!dest) return;
    if(!(dest->str)) return;
    if(!(*dest->str)) return;
    if(startindex < 0) return;
    if(endindex < startindex) return;
    int k = endindex;
    if(endindex > strlen(*src->str)) {
        k = strlen(*src->str);
    }


    dyn_str_clear(dest, resize);
    for(int i = startindex; i <= k; i++) {
        dyn_str_addchar(dest, *(*src->str + i));
    }

} 

int dyn_str_cmp(dynStr* str1, dynStr* str2) {
    return strcmp(*str1->str, *str2->str);
}

int dyn_str_cmp_str(dynStr* str1, char str2[]) {
    return strcmp(*str1->str, str2);
}

int __minval(int org, int min) {
    if(org < min) return min;
    return org;
}

// void dyn_str_move(dynStr* dyn_str, int startindex, int movement) {
//     if(!dyn_str) return;
//     if(!(dyn_str->str)) return;
//     if(!(*dyn_str->str)) return;
//     if(startindex < 0 || startindex > strlen(*dyn_str->str)) return;

//     while(strlen(*dyn_str->str) + movement > dyn_str->limit) {
//         *(dyn_str->limit) = *(dyn_str->limit) * 2;
//         *(dyn_str->str) = (char*)realloc(*(dyn_str->str), *(dyn_str->limit));
//     }

//     for(int i = startindex; i < strlen(*dyn_str->str); i++) {
//         *(*dyn_str->str + i) = 
//     }

// }