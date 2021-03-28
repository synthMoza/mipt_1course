#pragma once

#include "stack_macro.h"
#include "akinator.h"

#define HASH_CONST 31
#define STACK_SIZE_RANGE 100
#define STACK_NAME_SIZE 20
#define NUM_OF_CANARIES 1
#define STACK_CANARY_VALUE 1498009113567333171L
//#define SECURE_STACK
#define LOG_FILE_NAME "log.txt"

enum ERRORS {ER_STACK_UNDERFLOW = 1, ER_STACK_SIZE_NEGATIVE = 2, ER_STACK_DATA_NULLPTR = 3,
    ER_STACK_DIF_HASH = 4,ER_STACK_SIZE_OVERFLOW = 5, ER_STACK_LEFT_CANARY = 6, ER_STACK_RIGHT_CANARY = 7};

typedef struct Pair* Elem_t;

struct stack {
    char name[STACK_NAME_SIZE];
#ifdef SECURE_STACK
    unsigned long canary1[NUM_OF_CANARIES];
#endif
    Elem_t* data;
#ifdef SECURE_STACK
    long hash;
#endif
    size_t size;
    size_t maxsize;
#ifdef SECURE_STACK
    int errno;
    unsigned long canary2[NUM_OF_CANARIES];
#endif
};

typedef struct stack stack_t;

void stackDump(stack_t stk);
void stackConstruct(stack_t* stk);
long hashGenerate(stack_t stk);
void stackDestroy(stack_t* stk);
void stackClear(stack_t* stk);
void printPath(stack_t* stk);
void pinpointElement(tree* input_tree, char* name, stack_t* stk, stack_t* path);