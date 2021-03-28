#include "assembly.h"

struct token {
    enum token_type tokenType;
    union {
        enum cpu_cmd command;
        enum cpu_reg reg;
        int label_com;
        int number;
        enum ram_brackets bracket;
        enum ram_operation operation;
    };
};

struct label {
    char* name;
    int pos;
};

int main()
{
    FILE* output_code = NULL;
    unsigned int size_vec = 0;
    char* buf;
    vector_spec(token_t) vec_cmd;
    vector_init(&vec_cmd);
    vector_reserve(&vec_cmd, 1000);

    vector_spec(label_t) vec_labels;
    vector_init(&vec_labels);
    vector_reserve(&vec_labels, LABEL_MAX_NUMBER);

    unsigned int nLines = 0;
    char** commands = NULL;

    nLines = readCommands(&commands, &buf);
    tokenize_code((int) nLines, commands, &vec_cmd, &vec_labels);
    vector_destroy(&vec_cmd);
    vector_init(&vec_cmd);
    vector_reserve(&vec_cmd, 1000);
    tokenize_code((int) nLines, commands, &vec_cmd, &vec_labels);

    output_code = fopen("code.bin", "w");
    fwrite(vec_cmd.buf, vector_size(&vec_cmd), sizeof(token_t), output_code);
    fclose(output_code);
    vector_destroy(&vec_cmd);
    freeLabels(&vec_labels);
    vector_destroy(&vec_labels);
    free(buf);
    free(commands);
    return 0;
}

unsigned int readCommands(char*** commands, char** buf)
{
    FILE* input_code = NULL;
    size_t fileSize = 0;
    unsigned int nLines = 0;

    input_code = fopen(INPUT_FILE_NAME, "r");

    if (input_code == NULL)
    {
        printf("Error: can't open the file!\n");
        assert(0);
    }

    fileSize = file_size(input_code);
    *buf = (char*) calloc(fileSize + 2, sizeof(char));

    fread(*buf, fileSize, sizeof(char), input_code);
    nLines = set_string_ends(*buf, fileSize);

    *commands = (char**) calloc(nLines + 2, sizeof(char**));
    set_commands_from_buffer(*commands, *buf, fileSize);

    fclose(input_code);

    return nLines;
}

unsigned int set_string_ends(char* buf, unsigned int fileSize)
{
    unsigned int nLines = 0;
    unsigned int counter = 0;

    for (counter = 0; counter < fileSize; counter++)
    {
        if (buf[counter] == '\n')
        {
            nLines++;
            buf[counter] = '\0';
        }
    }

    buf[counter] = '\0';
    nLines++;

    return nLines;
}

void set_commands_from_buffer(char** commands, char* buf, unsigned int fileSize)
{
    int sym_counter = 0;
    int line_counter = 0;

    commands[line_counter++] = buf;

    for (sym_counter = 0; sym_counter < fileSize - 1; sym_counter++)
    {
        if (*buf == '\0')
        {
            commands[line_counter++] = buf + 1;
        }
        buf++;
    }
}

int get_token_cmd(char* str, vector* vec, vector_label* labels, int* var_counter)
{
    int n_consumed = 0;
    token_t token_got;
    enum cpu_cmd_fmt fmt_val = 0;
    const char* cmd_name;

    #define DEF_CMD(com, fmt)                           \
        sscanf(str, #com "%n", &n_consumed);            \
        if (n_consumed > 0)                             \
        {                                               \
            token_got.tokenType = TOKEN_CMD;            \
            token_got.command = CMD_##com;              \
            fmt_val = (fmt);                            \
            cmd_name = (#com);                          \
            goto token_comsumed;                        \
        }
    #include "def_com.h"
    #undef DEF_CMD
    return 0;
token_comsumed:
    str += n_consumed;
    vector_push_back(vec, token_got);

    if (fmt_val == CMDFMT_NOP)
        return 1;

    int nspaces = 0;

    sscanf(str, "%*[ \t\n]%n", &nspaces);
    if (nspaces == 0)
        goto tokenize_failed;

    str += nspaces;

    switch (fmt_val)
    {
        case CMDFMT_NUM :
            if (get_token_num(&str, vec) != 1)
                goto tokenize_failed;
            break;
        case CMDFMT_REG :
            if (get_token_reg(&str, vec) != 1)
                goto tokenize_failed;
            break;
        case CMDFMT_LABEL :
            if (get_token_label_go(str, vec, labels) != 1)
                goto tokenize_failed;
            break;
        case CMDFMT_RAM :
            if (get_token_ram(str, vec) != 1)
                goto tokenize_failed;
            break;
        default :
            assert(0);
    }
    return 1;
tokenize_failed:
    printf("Can't parse the argument for the command %s : %s\n", cmd_name, str);
    str -= n_consumed;
    return -1;
}

int get_token_num(char** str, vector* vec)
{
    int n_comsumed = 0;
    int inputNumer = 0;
    token_t got_token;

    sscanf(*str, "%d%n", &inputNumer, &n_comsumed);
    if (n_comsumed > 0)
    {
        got_token.tokenType = TOKEN_NUM;
        got_token.number = inputNumer;
        goto token_comsumed;
    }
    return 0;
token_comsumed:
    (*str) += n_comsumed;
    vector_push_back(vec, got_token);
    return 1;
}

int get_token_reg(char** str, vector* vec)
{
    int n_consumed = 0;
    token_t got_token;

    #define DEF_REG(name, val)                              \
        sscanf(*str, val "%n", &n_consumed);                \
        if (n_consumed > 0)                                 \
        {                                                   \
            got_token.tokenType = TOKEN_REG;                \
            got_token.reg = REG_##name;                     \
            goto token_consumed;                            \
        }
    #include "def_reg.h"
    #undef DEF_REG
    return 0;
token_consumed:
    (*str) += n_consumed;
    vector_push_back(vec, got_token);
    return 1;
}

int get_token_label_go(char* str, vector* vec, vector_label* labels)
{
    int n_consumed = 0;
    token_t got_token;
    int index = 0;
    char input_string[LABEL_MAX_SIZE];

    sscanf(str, "%s%n", input_string, &n_consumed);
    if (n_consumed > 0)
    {
        got_token.tokenType = TOKEN_LABEL_GO;
        index = searchLabel(labels, input_string);
        got_token.label_com = (index >= 0) ? labels->buf[index].pos : -1;
        goto token_consumed;
    }
    return 0;
token_consumed:
    vector_push_back(vec, got_token);
    return 1;
}

int searchLabel(vector_label* labels, const char* label)
{
    int counter = 0;

    for (counter = 0; counter < labels->size; counter++)
    {
        if (strcmp(labels->buf[counter].name, label) == 0)
        {
            return counter;
        }
    }

    return -1;
}

int get_token_label_def(char* str, vector* vec, vector_label * labels)
{
    enum token_type;
    int n_consumed = 0;
    int counter = 0;
    label_t got_label;
    char* input_string;

    input_string = (char*) calloc(LABEL_MAX_SIZE, sizeof(char));

    sscanf(str, ":%s%n", input_string, &n_consumed);
    if (n_consumed > 0)
    {
        got_label.name = input_string;
        got_label.pos = vec->size;
        goto token_consumed;
    }
    return 0;
token_consumed:
    vector_push_back(labels, got_label);
    return 1;
}

int get_token_ram(char* str, vector* vec)
{
    token_t got_token;
    int n_consumed = 0;

    if (get_token_ram_bracket(&str, vec) != 1)
    {
        printf("Error: expected '[.\n");
        assert(0);
    }

    if (get_token_reg(&str, vec) != 1) //not the register
    {
        if (get_token_num(&str, vec) != 1) //error case
        {
            printf("Error: expected register/num.\n");
            assert(0);
        }
        //now got the number
        goto got_adress;
    }
    //got the register
    if (get_token_op(&str, vec) != 1) //not the +/-
    {
        goto got_adress;
    }
    if (get_token_num(&str, vec) != 1) //error case
    {
        printf("Error: expected number after operation.\n");
        assert(0);
    }
    goto got_adress;
got_adress:
    if (get_token_ram_bracket(&str, vec) != 1)
    {
        printf("Error: expected '].\n");
        assert(0);
    }

    return 1;
}

int get_token_op(char** str, vector* vec)
{
    token_t got_token;
    int n_consumed = 0;
    enum ram_operation op;

    sscanf(*str, "+%n", &n_consumed);
    op = '+';

    if (n_consumed <= 0)
    {
        sscanf(*str, "-%n", &n_consumed);
        op = '-';

        if (n_consumed <= 0)
        {
            goto tokenize_failed;
        }
    }

    got_token.tokenType = TOKEN_OP;
    got_token.operation = op;
    goto token_consumed;
tokenize_failed:
    return -1;
token_consumed:
    (*str) += n_consumed;
    vector_push_back(vec, got_token);
    return 1;
}

int get_token_ram_bracket(char** str, vector* vec)
{
    token_t got_token;
    int n_consumed = 0;
    enum ram_brackets bracket = 0;

    sscanf(*str, "[%n", &n_consumed);
    bracket = openBracket;

    if (n_consumed <= 0)
    {
        sscanf(*str, "]%n", &n_consumed);
        bracket = closeBracket;

        if (n_consumed <= 0)
        {
            goto tokenize_failed;
        }
    }

    got_token.tokenType = TOKEN_RAM_BRACKET;
    got_token.bracket = bracket;
    goto token_consumed;
tokenize_failed:
    return -1;
token_consumed:
    (*str) += n_consumed;
    vector_push_back(vec, got_token);
    return 1;
}

void tokenize_code(int nLines, char** command, vector* vec, vector_label* labels)
{
    int counter = 0;
    int var_counter = 0;

    for (counter = 0; counter < nLines; counter++)
    {
        while(isspace(*(command[counter])))
            command[counter]++;
        if (*(command[counter]) == '\0')
            continue;
        if(!(get_token_cmd(command[counter], vec, labels, &var_counter) || get_token_label_def(command[counter], vec, labels)))
        {
            printf("Can't tokenize: %s", command[counter]);
            assert(0);
        }
    }
}

void freeLabels(vector_label* vec_labels)
{
    int counter = 0;
    int size = 0;

    size = vector_size(vec_labels);
    for (counter = 0; counter < size; counter++)
    {
        free(vec_labels->buf[counter].name);
    }
}