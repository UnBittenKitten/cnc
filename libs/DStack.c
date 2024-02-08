// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>

#define MAXSIZE 100

typedef struct {
    double arr[MAXSIZE];
    int elem;

} DStack;

DStack* dstack_init() {
    DStack* new = malloc(sizeof(DStack));
    if(!new) {
        printf("Error: Unable to create stack\n");
        return 0;
    }
    new->elem = 0;
    return new;
}

void dstack_push(DStack* stack, double num) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return;
    }

    if(stack->elem >= MAXSIZE) {
        printf("Error: Stack overflow\n");
        return;
    }
    ((stack->arr)[stack->elem]) = num;
    stack->elem = stack->elem + 1;
}

double dstack_pop(DStack* stack) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return 0;
    }

    if(stack->elem <= 0) {
        printf("Error: Stack underflow\n");
        return 0;
    }
    (stack->elem) = (stack->elem) - 1;
    double temp = ((stack->arr)[(stack->elem)]);
    ((stack->arr)[(stack->elem) + 1]) = 0;
    return temp;
}

void dstack_print(DStack* stack) {
    for(int i = 0; i < stack->elem; i++) {
        printf("%.0lf ", (stack->arr)[i]);
    }
}

void dstack_free(DStack* stack) {
    free(stack);
}

int dstack_search(DStack* stack, double search) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return -1;
    }
    for(int i = 0; i < stack->elem; i++) {
        if(search == ((stack->arr)[i])) {
            return i;
        }
    }
    printf("Error: Element not found\n");
    return -1;
} 

void dstack_searchdel(DStack* stack, double search) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return;
    }

    int i = dstack_search(stack, search);
    if(i < 0) return;

    (stack->arr)[i] = 0;
    stack->elem--;
    for(int j = i; j < stack->elem; j++) {
        (stack->arr)[j] = (stack->arr)[j + 1];
    }

    return;
}

int isSorted(DStack* stack) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return 0;
    }
    for(int i = 0; i < stack->elem - 1; i++) {
        if((stack->arr)[i] > (stack->arr)[i + 1])
        return 0;
    }
    return 1;
}

int isEmpty(DStack* stack) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return 0;
    }

    if(stack->elem == 0) return 1;
    return 0;
}

void dstack_sort(DStack* stack) {
    if(!stack) {
        printf("Error: Stack not found\n");
        return;
    }

    DStack* new = dstack_init();
    double temp;

    while(!isEmpty(stack)) {
        temp = (stack->arr)[0];
        for(int i = 0; i < stack->elem; i++) {
            if(((stack->arr)[i]) <= temp) {
                temp = ((stack->arr)[i]);
            }

        }
        dstack_push(new, temp);
        dstack_searchdel(stack, temp);
    }

    memcpy(stack, new, sizeof(DStack));
    dstack_free(new);

}