#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "stack/stack.c"
#include "cvector.h"
#include "cpu_macro.h"

typedef struct token token_t;
typedef struct cpu cpu_t;

int readCommands(token_t** commands);
void cpu_construct(cpu_t* scpu);
void cpu_destroy(cpu_t* scpu);
int cpu_tick(cpu_t* scpu, int* curCommand);
int get_inst_reg_id(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_push(cpu_t* scpu,  int* curCommand);
static inline int cpu_cmd_pushr(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_pop(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_popr(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_in(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_out(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_add(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_mul(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_div(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_sub(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_sqrt(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_jmp(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_ja(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_jb(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_jae(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_jbe(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_je(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_jne(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_call(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_ret(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_pushm(cpu_t* scpu, int* curCommand);
static inline int cpu_cmd_popm(cpu_t* scpu, int* curCommand);

enum cpu_cmd {
    #define DEF_CMD(com, fmt) CMD_##com,
    #include "/home/synthmoza/CLionProjects/myAssembler/def_com.h"
    #undef DEF_CMD
    CMD_NUM = 21
};

enum cpu_reg {
    #define DEF_REG(name, str) REG_##name,
    #include "/home/synthmoza/CLionProjects/myAssembler/def_reg.h"
    #undef DEF_REG
    REG_NUM = 5
};

enum cpu_cmd_fmt {
    CMDFMT_NOP, CMDFMT_NUM, CMDFMT_REG, CMDFMT_LABEL, CMDFMT_RAM
};

enum token_type {
    TOKEN_NOP, TOKEN_NUM, TOKEN_CMD, TOKEN_REG, TOKEN_LABEL_DEF, TOKEN_LABEL_GO, TOKEN_RAM_BRACKET, TOKEN_OP
};

enum ram_brackets {
    openBracket = '[', closeBracket = ']'
};

enum ram_operation {
    OP_PLUS = '+', OP_MINUS = '-'
};