#include "dif.h"

struct tree {
    struct tree* leftChild;
    struct tree* rightChild;
    int valueType;
    int value;
};

struct token {
    enum tokenTypes tokenType;
    union {
        enum var_id variable;
        enum const_id constant;
        double number;
        enum bracket_id bracket;
        enum dif_op_id operation;
        enum pow_op pow;
    };
};

int exp_counter = 0;

int main() {
    char* input_string = NULL;
    Tree_t* expression = NULL;
    Tree_t* derivative = NULL;
    Tree_t* copied_tree = NULL;
    Tree_t* temp_tree = NULL;
    vector_spec(token_t) vec;
    vector_init(&vec);
    vector_reserve(&vec, 100);

    FILE* output_file = fopen("output.tex", "w");

    input_string = getLine();
    tokenize_string(&input_string, &vec);
    expression = getTree(&vec);
    derivative = diffExpression(expression);

    copied_tree = derivative;

    for (int count = 0; count < OPTIMIZE_COUNT; count++)
    {
        temp_tree = copied_tree;
        copied_tree = optimizeTree(copied_tree);
        freeNode(temp_tree);
    }

    graphviz_visualize_tree(copied_tree);

    latexOutput(expression, copied_tree, output_file);
    fclose(output_file);
    freeNode(expression);
    freeNode(copied_tree);
    return 0;
}

char* getLine(void)
{
    char inputString[256];
    char* string = NULL;
    int counter = 0;
    char c = 0;

    while((c = getchar()) != '\n')
        inputString[counter++] = c;

    if (c == '\n')
        inputString[counter++] = '\0';

    string = (char*) calloc(counter, sizeof(char));
    memcpy(string, inputString, counter);

    return string;
}

int get_op_token(char **str, vector *vec) {
    enum dif_op_id;
    int consumed = 0;
    token_t token_got;

    #define DEF_OP(name, string, diff)                                              \
            sscanf(*str, string "%n", &consumed);                                   \
            if (consumed > 0)                                                       \
            {                                                                       \
                token_got.tokenType = TOKEN_OP;                                     \
                token_got.operation = OP_##name;                                    \
                goto token_consumed;                                                \
            }
    #include "dif_op_def.h"
    #undef DEF_OP
    return 0;
token_consumed:
    (*str) += consumed;
    vector_push_back(vec, token_got);
    return 1;
}

int get_num_token(char** str, vector* vec)
{
    int consumed = 0;
    token_t token_got;
    double value = 0;

    sscanf(*str, "%lg%n", &value, &consumed);
    if (consumed > 0)
    {
        token_got.number = value;
        token_got.tokenType = TOKEN_NUM;
        goto token_consumed;
    }
    return 0;
token_consumed:
    (*str) += consumed;
    vector_push_back(vec, token_got);
    return 1;
}

int get_var_token(char** str, vector* vec)
{
    int consumed = 0;
    token_t token_got;

    sscanf(*str, "x%n",&consumed);
    if (consumed > 0)
    {
        token_got.variable = 'x';
        token_got.tokenType = TOKEN_VAR;
        goto token_consumed;
    }
    return 0;
token_consumed:
    (*str) += consumed;
    vector_push_back(vec, token_got);
    return 1;
}

int get_brackets_token(char** str, vector* vec)
{
    int consumed = 0;
    token_t token_got;
    char bracket = 0;

    sscanf(*str, "(%n", &consumed);
    bracket = '(';
    if (consumed == 0) {
        sscanf(*str, ")%n", &consumed);
        bracket = ')';
    }

    if (consumed > 0)
    {
        token_got.bracket = bracket;
        token_got.tokenType = TOKEN_SYN;
        goto token_consumed;
    }
    return 0;

token_consumed:
    (*str) += consumed;
    vector_push_back(vec, token_got);
    return 1;
}

int get_token_const(char** str, vector* vec)
{
    enum const_id;
    int consumed = 0;
    token_t token_got;

    #define DEF_CONST(name, string)                                                 \
            sscanf(*str, string "%n", &consumed);                                   \
            if (consumed > 0)                                                       \
            {                                                                       \
                token_got.tokenType = TOKEN_CONST;                                  \
                token_got.constant = CONST_ID_##name;                               \
                goto token_consumed;                                                \
            }
    #include "def_const.h"
    #undef DEF_CONST

    return 0;

    token_consumed:
    (*str) += consumed;
    vector_push_back(vec, token_got);
    return 1;
}

void tokenize_string(char** string, vector* vec)
{
    while (**string != '\0')
    {
        if (!(get_brackets_token(string, vec) || get_op_token(string, vec)
        || get_var_token(string, vec) || get_num_token(string, vec) || get_token_const(string, vec))) {
            printf("Can't tokenize: %s", *string);
            assert(0);
        }
    }
}

Tree_t* getNode(void)
{
    Tree_t* temp = NULL;

    temp = (Tree_t*) calloc(1, sizeof(Tree_t));

    return temp;
}

Tree_t* getTree(vector* vec)
{
    Tree_t* val = NULL;

    val = getPlusMinus(vec);

    if(vec->size != exp_counter)
    {
        printf("Syntax error: expected '\\0'\n");
        assert(0);
    }
    return val;
}

Tree_t* getNumberVar(vector* vec)
{
    Tree_t* getnumber_tree = NULL;

    getnumber_tree = getNode();

    switch(vec->buf[exp_counter].tokenType) {
        case TOKEN_NUM :
            getnumber_tree->valueType = TOKEN_NUM;
            getnumber_tree->value = vec->buf[exp_counter++].number;
            break;
        case TOKEN_VAR:
            getnumber_tree->valueType = TOKEN_VAR;
            getnumber_tree->value = vec->buf[exp_counter++].variable;
            break;
        case TOKEN_CONST :
            getnumber_tree->valueType = TOKEN_CONST;
            getnumber_tree->value = vec->buf[exp_counter++].constant;
            break;
        default:
            printf("Error: expected number/variable");
            assert(0);
    }

    return getnumber_tree;
}

Tree_t* getBrackets_Num(vector* vec)
{
    Tree_t* getbracketsnum_tree = NULL;

    if (vec->buf[exp_counter].tokenType == TOKEN_NUM || vec->buf[exp_counter].tokenType == TOKEN_VAR || vec->buf[exp_counter].tokenType == TOKEN_CONST)
    {
        getbracketsnum_tree = getNumberVar(vec);
        return getbracketsnum_tree;
    } else if (vec->buf[exp_counter].tokenType == TOKEN_SYN && vec->buf[exp_counter].bracket == '(') {
        exp_counter++;
        getbracketsnum_tree = getPlusMinus(vec);
        if (vec->buf[exp_counter].tokenType == TOKEN_SYN && vec->buf[exp_counter].bracket == ')') {
            exp_counter++;
            return getbracketsnum_tree;
        } else {
            printf("Syntax error: expected ')'.\n");
            assert(0);
        }
    }

    return getbracketsnum_tree;
}

Tree_t* getMathFunc(vector* vec)
{
    enum dif_op_id;
    Tree_t* getmathfunc_main_tree = NULL;
    Tree_t* getmathfunc_value_tree = NULL;

    if (vec->buf[exp_counter].tokenType == TOKEN_SYN || vec->buf[exp_counter].tokenType == TOKEN_NUM || vec->buf[exp_counter].tokenType == TOKEN_VAR || vec->buf[exp_counter].tokenType == TOKEN_CONST)
    {
        getmathfunc_main_tree = getBrackets_Num(vec);
    }
    else
    {
        getmathfunc_main_tree = getNode();
        getmathfunc_main_tree->valueType = TOKEN_OP;

        getmathfunc_main_tree->value = vec->buf[exp_counter].operation;
        exp_counter++;
        if (vec->buf[exp_counter].tokenType != TOKEN_SYN || vec->buf[exp_counter].bracket != '(')
        {
            printf("Syntax error: expected <function>(expression).\n");
            assert(0);
        }
        else
        {
            exp_counter++;
            getmathfunc_value_tree = getPlusMinus(vec);

            if (vec->buf[exp_counter].tokenType != TOKEN_SYN || vec->buf[exp_counter].bracket != ')')
            {
                printf("Syntax error: expected <func>(expression).\n");
                assert(0);
            }

            getmathfunc_main_tree->leftChild = getmathfunc_value_tree;

            exp_counter++;
        }
    }

    return getmathfunc_main_tree;
}

Tree_t* getPow(vector* vec)
{
    enum dif_op_id;
    Tree_t* getpow_op = NULL;
    Tree_t* getpow_main = NULL;

    getpow_main = getMathFunc(vec);

    if (vec->buf[exp_counter].tokenType == TOKEN_OP && vec->buf[exp_counter].pow == OP_POW)
    {
        int op = 0;

        op = vec->buf[exp_counter++].pow;

        getpow_op = getNode();
        getpow_op->valueType = TOKEN_OP;
        getpow_op->value = op;
        getpow_op->leftChild = getpow_main;
        getpow_op->rightChild = getMathFunc(vec);

        getpow_main = getpow_op;
    }

    return getpow_main;
}

Tree_t* getMulDiv(vector* vec)
{
    Tree_t* getmuldiv_value1_tree = NULL;
    Tree_t* getmuldiv_value2_tree = NULL;
    Tree_t* getmuldiv_op_tree = NULL;
    Tree_t* getmuldiv_main_tree = NULL;

    getmuldiv_value1_tree = getPow(vec);
    getmuldiv_main_tree = getmuldiv_value1_tree;

    while (vec->buf[exp_counter].tokenType == TOKEN_OP && (vec->buf[exp_counter].operation == OP_MUL || vec->buf[exp_counter].operation == OP_DIV))
    {
        int op = 0;

        op = vec->buf[exp_counter++].operation;

        getmuldiv_value2_tree = getPow(vec);

        getmuldiv_op_tree = getNode();

        getmuldiv_op_tree->valueType = TOKEN_OP;
        getmuldiv_op_tree->value = op;
        getmuldiv_op_tree->leftChild = getmuldiv_value1_tree;
        getmuldiv_op_tree->rightChild = getmuldiv_value2_tree;

        getmuldiv_main_tree = getmuldiv_op_tree;
        getmuldiv_value1_tree = getmuldiv_op_tree;
    }

    return getmuldiv_main_tree;
}

Tree_t* getPlusMinus(vector* vec)
{
    Tree_t* getplusminus_op_tree = NULL;
    Tree_t* getplusminus_value1_tree = NULL;
    Tree_t* getplusminus_value2_tree = NULL;
    Tree_t* getplusminus_main_tree = NULL;

    getplusminus_value1_tree = getMulDiv(vec);
    getplusminus_main_tree = getplusminus_value1_tree;

    while(vec->buf[exp_counter].tokenType == TOKEN_OP && (vec->buf[exp_counter].operation == OP_PLUS || vec->buf[exp_counter].operation == OP_MINUS))
    {
        int op = 0;

        op = vec->buf[exp_counter++].operation;

        getplusminus_value2_tree = getMulDiv(vec);

        getplusminus_op_tree = getNode();
        getplusminus_op_tree->valueType = TOKEN_OP;
        getplusminus_op_tree->value = op;
        getplusminus_op_tree->leftChild = getplusminus_value1_tree;
        getplusminus_op_tree->rightChild = getplusminus_value2_tree;

        getplusminus_value1_tree = getplusminus_op_tree;
        getplusminus_main_tree = getplusminus_op_tree;
    }

    return getplusminus_main_tree;
}

void graphviz_visualize_tree(Tree_t* tree)
{
    FILE* output_file = NULL;

    output_file = freopen(GRAPH_DOT_FILE_NAME, "w", stdout);

    printf("digraph expression_tree {\n");
    graphviz_print_node(tree, 0);
    printf("}");

    fclose(output_file);

    //system("dot -Tpng expression_tree.gv -o expression_tree.png");
}

int label_counter = 1;

void graphviz_print_node(Tree_t* node, int father)
{
    if (node->leftChild != NULL)
    {
        printf("\t node%d -> node%d\n", father, label_counter);
        graphviz_type_node_print(node, father);
        graphviz_type_node_print(node->leftChild, label_counter);
        graphviz_print_node(node->leftChild, label_counter++);
    }
    if (node->rightChild != NULL)
    {
        printf("\t node%d -> node%d\n", father, label_counter);
        graphviz_type_node_print(node, father);
        graphviz_type_node_print(node->rightChild, label_counter);
        graphviz_print_node(node->rightChild, label_counter++);
    }
}

void graphviz_type_node_print(Tree_t* node, int label_num)
{
    enum dif_op_id;
    switch(node->valueType) {
        case TOKEN_NUM :
            printf("\tnode%d [shape=box,label=\"%d\"]\n", label_num, node->value);
            break;
        case TOKEN_CONST :
            #define DEF_CONST(name, str)                                                 \
                if ((node)->value == CONST_ID_##name)                                    \
                {                                                                        \
                    printf("\tnode%d [shape=circle,label=\"%s\"]\n", label_num, (str));  \
                    goto op_succes;                                                      \
                }
            #include "def_const.h"
            #undef DEF_CONST
            printf("UNKNOWN OPERATION SORRY\n");
            op_succes:
            break;
        case TOKEN_VAR :
            printf("\tnode%d [shape=diamond,label=\"%c\"]\n", label_num, (node)->value);
            break;
        case TOKEN_OP :
            #define DEF_OP(name, str, diff)                                 \
                if ((node)->value == OP_##name)                             \
                {                                                           \
                    printf("\tnode%d [shape=circle,label=\"%s\"]\n", label_num, (str));  \
                    goto op_succes0;                                         \
                }
            #include "dif_op_def.h"
            #undef DEF_OP
            printf("UNKNOWN OPERATION SORRY\n");
            op_succes0:
            break;
    }
}

Tree_t* copyTree(Tree_t* tree)
{
    assert(tree != NULL);

    Tree_t* copied_node = NULL;

    copied_node = (Tree_t*) calloc(1, sizeof(Tree_t));
    memcpy(copied_node, tree, sizeof(Tree_t));

    if (tree->leftChild != NULL)
    {
        copied_node->leftChild = copyTree(tree->leftChild);
    }

    if (tree->rightChild != NULL)
    {
        copied_node->rightChild = copyTree(tree->rightChild);
    }

    return copied_node;
}

Tree_t* diffExpression(Tree_t* expression)
{
    enum dif_op_id;
    Tree_t* diff_tree = NULL;

    switch (expression->valueType) {
        case TOKEN_NUM :
        case TOKEN_CONST :
            diff_tree = createNode(TOKEN_NUM, 0, NULL, NULL);
            break;
        case TOKEN_VAR :
            diff_tree = createNode(TOKEN_NUM, 1, NULL, NULL);
            break;
        case TOKEN_OP :
            #define DEF_OP(name, str, diff) if (expression->value == OP_##name) {       \
                diff_tree = diff(expression);                                           \
                goto op_diff;                                                           \
            }
            #include "dif_op_def.h"
            #undef DEF_OP
            op_diff:
            break;
        default :
            printf("Syntax error: unknown token");
            assert(0);
    }

    return diff_tree;
}

void freeNode(Tree_t* input_tree)
{
    if (input_tree->leftChild == NULL && input_tree->rightChild == NULL) {
        free(input_tree);
        return ;
    }
    else {
        if (input_tree->leftChild != NULL)
            freeNode(input_tree->leftChild);

        if (input_tree->rightChild != NULL)
            freeNode(input_tree->rightChild);
    }

    free(input_tree);
}

void latexOutput(Tree_t* expression, Tree_t* derivative, FILE* output_file)
{
    fprintf(output_file, "\\documentclass[12pt,a4paper]{scrartcl}\n");
    fprintf(output_file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(output_file, "\\usepackage[english]{babel}\n");
    fprintf(output_file, "\\usepackage{indentfirst}\n");
    fprintf(output_file, "\\usepackage{amsmath}\n");
    fprintf(output_file, "\\begin{document}\n");
    fprintf(output_file, "The original function is\n");
    fprintf(output_file, "\\begin{equation}\n");
    fprintf(output_file, "f(x) = ");
    latexPrint(expression, output_file);
    fprintf(output_file, "\\end{equation}\n");
    fprintf(output_file, "The derivative of the function is\n");
    fprintf(output_file, "\\begin{equation}\n");
    fprintf(output_file, "f'(x) = ");
    latexPrint(derivative, output_file);
    fprintf(output_file, "\n");
    fprintf(output_file, "\\end{equation}\n");
    fprintf(output_file, "\\end{document}\n");
}

void latexPrint(Tree_t* expression, FILE* output_file)
{
    enum dif_op_id;
    enum const_id;
    switch (expression->valueType) {
        case TOKEN_NUM :
            if (expression->value >= 0)
                fprintf(output_file, "%d", expression->value);
            else
                fprintf(output_file, "(%d)", expression->value);
            break;
        case TOKEN_VAR :
            fprintf(output_file, "%c", expression->value);
            break;
        case TOKEN_CONST :
                #define DEF_CONST(name, str)                    \
                if (expression->value == CONST_ID_##name) {     \
                    latex_##name(expression);                   \
                    goto written0;                               \
                }
                #include "def_const.h"
                #undef DEF_CONST
                printf("Error: unknown constant\n");
                assert(0);
                written0:
                break;
        case TOKEN_OP :
            #define DEF_OP(name, str, diff)                     \
                if (expression->value == OP_##name) {           \
                    latex_##name(expression);                   \
                    goto written;                               \
                }
            #include "dif_op_def.h"
            #undef DEF_OP
            written:
            break;
    }
}

Tree_t* optimizeTree(Tree_t* expression)
{
    assert(expression != NULL);

    enum dif_op_id;
    Tree_t* ret_tree = NULL;

    switch (expression->valueType)
    {
        case TOKEN_OP :
            switch (expression->value) {
                case OP_PLUS :
                    if (expression->leftChild->valueType == TOKEN_NUM && expression->leftChild->value == 0)
                    {
                        ret_tree = optimizeTree(expression->rightChild);
                    } else
                    if (expression->rightChild->valueType == TOKEN_NUM && expression->rightChild->value == 0)
                    {
                        ret_tree = optimizeTree(expression->leftChild);
                    }
                    break;
                case OP_MINUS :
                    if (expression->rightChild->valueType == TOKEN_NUM && expression->rightChild->value == 0)
                    {
                        ret_tree = optimizeTree(expression->leftChild);
                    }
                    break;
                case OP_MUL :
                    if ((expression->leftChild->valueType == TOKEN_NUM && expression->leftChild->value == 0) ||
                    (expression->rightChild->valueType == TOKEN_NUM && expression->rightChild->value == 0))
                    {
                        ret_tree = createNode(TOKEN_NUM, 0, NULL, NULL);
                    }
                    else if (expression->rightChild->valueType == TOKEN_NUM && expression->rightChild->value == 1)
                    {
                        ret_tree = copyTree(expression->leftChild);
                    }
                    else if (expression->leftChild->valueType == TOKEN_NUM && expression->leftChild->value == 1)
                    {
                        ret_tree = copyTree(expression->rightChild);
                    }
                    break;
                case OP_DIV :
                    if (expression->rightChild->valueType == TOKEN_NUM && expression->rightChild->value == 1)
                    {
                        ret_tree = copyTree(expression->leftChild);
                    }
                    else if (expression->leftChild->valueType == TOKEN_NUM && expression->leftChild->value == 0)
                    {
                        ret_tree = createNode(TOKEN_NUM, 0, NULL, NULL);
                    }
                    break;
            }
            break;
        case TOKEN_NUM :
        case TOKEN_VAR :
        case TOKEN_CONST :
            ret_tree = copyTree(expression);
            break;
        default :
            printf("Error: unknown token type.");
            assert(0);
    }

    if (ret_tree == NULL) {
        ret_tree = createNode(expression->valueType, expression->value, NULL, NULL);
        if (expression->leftChild != NULL)
            ret_tree->leftChild = optimizeTree(expression->leftChild);
        if (expression->rightChild != NULL)
            ret_tree->rightChild = optimizeTree(expression->rightChild);
    }

    return ret_tree;
}