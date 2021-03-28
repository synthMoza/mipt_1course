#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "dif_macro.h"
#include <stdbool.h>
#include <math.h>
#include "cvector.h"

typedef struct tree Tree_t;
typedef struct token token_t;
typedef vector_define(token_t) vector;

Tree_t* getTree(vector* vec);
Tree_t* getPlusMinus(vector* vec);
Tree_t* getMulDiv(vector* vec);
Tree_t* getNode(void);
Tree_t* getNumberVar(vector* vec);
Tree_t* getBrackets_Num(vector* vec);
int get_op_token(char **str, vector *buf);
int get_num_token(char** str, vector* vec);
int get_var_token(char** str, vector* vec);
int get_token_const(char** str, vector* vec);
void tokenize_string(char** string, vector* vec);
Tree_t* copyTree(Tree_t* tree);
char* getLine(void);
void graphviz_print_node(Tree_t* node, int father);
void graphviz_visualize_tree(Tree_t* tree);
void graphviz_type_node_print(Tree_t* node, int label_num);
Tree_t* diffExpression(Tree_t* expression);
void freeNode(Tree_t* input_tree);
Tree_t* optimizeTree(Tree_t* expression);
void latexPrint(Tree_t* expression, FILE* output_file);
void latexOutput(Tree_t* expression, Tree_t* derivative, FILE* output_file);

enum dif_op_id {
    #define DEF_OP(name, str, dif) OP_##name,
    #include "dif_op_def.h"
    #undef DEF_OP
    OP_NUM
};

enum tokenTypes {
    TOKEN_NOPE, TOKEN_OP, TOKEN_NUM, TOKEN_VAR, TOKEN_SYN, TOKEN_CONST
};

enum pow_op {
    powSymbol = '^'
};

enum bracket_id {
    openBracket = '(', closeBracket = ')'
};

enum var_id {
    var_x = 'x'
};

enum const_id {
    #define DEF_CONST(name, str) CONST_ID_##name,
    #include "def_const.h"
    #undef DEF_CONST
    CONST_NUM
};