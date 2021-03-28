#include "stack_macro.h"

#define HASH_CONST 31
#define STACK_SIZE_RANGE 100
#define STACK_NAME_SIZE 20
#define NUM_OF_CANARIES 1
#define STACK_CANARY_VALUE 1498009113567333171L
#define LOG_FILE_NAME "log.txt"

enum ERRORS {ER_STACK_UNDERFLOW = 1, ER_STACK_SIZE_NEGATIVE = 2, ER_STACK_DATA_NULLPTR = 3,
    ER_STACK_DIF_HASH = 4,ER_STACK_SIZE_OVERFLOW = 5, ER_STACK_LEFT_CANARY = 6, ER_STACK_RIGHT_CANARY = 7};

typedef int Elem_t;
typedef struct stack stack_t;

void stackDump(stack_t stk);
void stackConstruct(stack_t* stk);
long hashGenerate(stack_t stk);
void stackDestroy(stack_t* stk);
void stackClear(stack_t* stk);


