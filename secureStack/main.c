#include "stack.h"

int main() {
    stack_t* stack = (stack_t*) calloc (1, sizeof(*stack));
    stackConstruct(stack);
    STACK_PUSH(stack, 5);
    printf("THE FIRST ELEMENT IS %d\n", STACK_POP(stack));
    stackDestroy(stack);
    free(stack);
    return 0;
}