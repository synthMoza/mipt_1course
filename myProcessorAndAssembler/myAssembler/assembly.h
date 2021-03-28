#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "assembly_macro.h"
#include "cvector.h"

typedef struct token token_t;
typedef vector_define(token_t) vector;
typedef struct label label_t;
typedef vector_define(label_t) vector_label;

unsigned int readCommands(char*** commands, char** buf);
unsigned int set_string_ends(char* buf, unsigned int fileSize);
void set_commands_from_buffer(char** commands, char* buf, unsigned int fileSize);
int get_token_cmd(char* str, vector* vec, vector_label* labels, int* var_counter);
int get_token_num(char** str, vector* vec);
int get_token_reg(char** str, vector* vec);
int get_token_label_go(char* str, vector* vec, vector_label* labels);
int get_token_label_def(char* str, vector* vec, vector_label * labels);
int get_token_ram(char* str, vector* vec);
int get_token_ram_bracket(char** str, vector* vec);
int get_token_op(char** str, vector* vec);
void tokenize_code(int nLines, char** command, vector* vec, vector_label* labels);
void freeLabels(vector_label* vec_labels);

int searchLabel(vector_label* labels, const char* label);

enum cpu_cmd {
    #define DEF_CMD(com, fmt)  CMD_##com,
    #include "def_com.h"
    #undef DEF_CMD
    CMD_NUM = 21
};

enum cpu_reg {
    #define DEF_REG(name, str) REG_##name,
    #include "def_reg.h"
    #undef DEF_REG
    REG_NUM = 4
};

enum ram_brackets {
    openBracket = '[', closeBracket = ']'
};

enum ram_operation {
    OP_PLUS = '+', OP_MINUS = '-'
};

enum cpu_cmd_fmt {
    CMDFMT_NOP, CMDFMT_NUM, CMDFMT_REG, CMDFMT_LABEL, CMDFMT_RAM
};

enum token_type {
    TOKEN_NOP, TOKEN_NUM, TOKEN_CMD, TOKEN_REG, TOKEN_LABEL_DEF, TOKEN_LABEL_GO, TOKEN_RAM_BRACKET, TOKEN_OP
};