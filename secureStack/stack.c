#include "stack.h"

void stackConstruct(stack_t* stk) {
    assert(stk);

    int counter = 0;

    stk->size = 0;

    stk->data = (Elem_t*) calloc(STACK_SIZE_RANGE, sizeof(Elem_t));
    stk->maxsize = STACK_SIZE_RANGE;

#ifdef SECURE_STACK
    for (counter = 0; counter < NUM_OF_CANARIES; counter++) {
        stk->canary1[counter] = STACK_CANARY_VALUE;
        stk->canary2[counter] = STACK_CANARY_VALUE;
    }

    stk->errno = 0;
    stk->hash = hashGenerate(*stk);
#endif
}
#ifdef SECURE_STACK
void stackDump(stack_t stk) {
    int counter = 0;

    FILE* log_file = freopen(LOG_FILE_NAME, "w", stdout);

    if (stk.errno == 0)
        printf("stack_t \"%s\" [%p] (ok)\n", stk.name, &stk);
    else
        printf("stack_t \"%s\" [%p] (ERROR)\n", stk.name, &stk);
    printf("\t{\n");
    printf("\t\terrcode = %d\n", stk.errno);
    printf("\t\tsize = %zu\n", stk.size);
    printf("\t\thashcode = 0x%lx\n", stk.hash);
    printf("\t\tcanary_original = %lu\n", STACK_CANARY_VALUE);
    if (NUM_OF_CANARIES == 1) {
        printf("\t\tcanary1 = %lu\n", stk.canary1[0]);
        printf("\t\tcanary2 = %lu\n", stk.canary2[0]);
    } else {
        for (counter = 0; counter < NUM_OF_CANARIES; counter++) {
            printf("\t\tcanary1[%d] = %lu\n", counter, stk.canary1[counter]);
            printf("\t\tcanary2[%d] = %lu\n", counter, stk.canary2[counter]);
        }
    }
    printf("\t\tdata[%zu] = [%p]\n", stk.maxsize, stk.data);
    printf("\t\t\t{\n");
    for (counter = 0; counter < stk.maxsize; counter++) {
        if (counter < stk.size) {
            printf("\t\t\t\t*[%d] = %d\n", counter, stk.data[counter]);//CHANGE THIS LINE IF YOU CHANGE THIS!!!!!!!!
        }
        else {
            printf("\t\t\t\t [%d] = %d\n", counter, stk.data[counter]);
        }

    }
    printf("\t\t\t}\n");
    printf("\t}\n");

    fclose(log_file);
}

long hashGenerate(stack_t stk) {
    //type of hash: hash =  size * p + maxsize * p + errno * p + stk[0] * 1^2 * size + stk[2] * 3^2 * size + ... + stk[size - 1] * (size) ^ 2 * size
    int counter = 0;
    unsigned long hash = stk.size * HASH_CONST + stk.maxsize * HASH_CONST + stk.errno * HASH_CONST;

    for (counter = 0; counter < stk.size; counter++) {
        hash += (stk.data[counter] * (counter + 1) * (counter + 1)) * stk.size;
    }

    return (long) hash;
}
#endif
void stackDestroy(stack_t* stk) {
    assert(stk);

    int counter = 0;

    free(stk->data);

#ifdef SECURE_STACK
    stk->hash = 0;

    for (counter = 0; counter < NUM_OF_CANARIES; counter++) {
        stk->canary1[counter] = 0;
        stk->canary2[counter] = 0;
    }

    stk->errno = 0;
    stk->size = 0;
#endif

    stk->maxsize = 0;
}

void stackClear(stack_t* stk) {
    if (STACK_OK(stk) == 0)
    {
        stackDump(*stk);
        assert(!"OK");
    }

    free(stk->data);
    stackConstruct(stk);
}