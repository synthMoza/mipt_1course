#include "cpu.h"

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

struct cpu {
    stack_t stk;
    stack_t stk_ret;
    int registers[REG_NUM];
    token_t* rip;
    int* RAM;
};

int main() {
    cpu_t scpu;
    int nCommands = 0;
    token_t* commands = NULL;
    int counter = 0;
    int sign = 0;

    cpu_construct(&scpu);
    nCommands = readCommands(&commands);
    scpu.rip = commands;

    for(counter = 0; counter < nCommands; counter++, scpu.rip++)
    {
        sign = cpu_tick(&scpu, &counter);
        if (sign == 2)
            break;
        if (sign != 1)
        {
            printf("Can't execute the command number %d.\n", counter);
            assert(0);
        }
    }

    cpu_destroy(&scpu);
    free(commands);
    return 0;
}

int readCommands(token_t** commands)
{
    FILE* input_code = NULL;
    int nCommands = 0;
    size_t fileSize = 0;

    input_code = fopen(INPUT_FILE_NAME, "r");

    if (input_code == NULL)
    {
        printf("Error: can't open the file!\n");
        assert(0);
    }

    fileSize = file_size(input_code);
    nCommands = fileSize / sizeof(token_t);
    *commands = (token_t*) calloc(nCommands + 2, sizeof(token_t));
    fread(*commands, nCommands, sizeof(token_t), input_code);

    fclose(input_code);

    return nCommands;
}

void cpu_construct(cpu_t* scpu)
{
    int counter = 0;
    stackConstruct(&(scpu->stk));
    stackConstruct(&(scpu->stk_ret));

    for (counter = 0; counter < REG_NUM; counter++)
    {
        scpu->registers[counter] = 0;
    }

    scpu->RAM = (int*) calloc(CPU_RAM_SIZE, sizeof(int));
}

void cpu_destroy(cpu_t* scpu)
{
    stackDestroy(&(scpu->stk));
    stackDestroy(&(scpu->stk_ret));

    free(scpu->RAM);
}

int cpu_tick(cpu_t* scpu, int* curCommand)
{
    int sign = 0;
    if (scpu->rip->tokenType != TOKEN_CMD)
        return -1; //not the command

    #define DEF_CMD(com, fmt)                                   \
    if (scpu->rip->command == CMD_##com) {                      \
        sign = cpu_cmd_##com(scpu, curCommand);                 \
        if (sign == 2)                                          \
            goto cmd_end;                                       \
        if (sign > 0) {                                         \
            goto cmd_done;                                      \
        }                                                       \
        else                                                    \
        {                                                       \
            printf("Error: unknown command.\n");                \
            assert(0);                                          \
        }                                                       \
    }
    #include "/home/synthmoza/CLionProjects/myAssembler/def_com.h"
    #undef DEF_CMD
    return 0;
cmd_done:
    return 1;
cmd_end:
    return 2;
}

static inline int cpu_cmd_push(cpu_t* scpu, int* curCommand)
{
    int inputNumber = 0;

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_NUM)
    {
        return -1;
    }
    inputNumber = scpu->rip->number;
    STACK_PUSH(&(scpu->stk), inputNumber);
    return 1;
}

static inline int cpu_cmd_popr(cpu_t* scpu, int* curCommand)
{
    int regNumber = 0;
    int regIndex = 0;

    regNumber = STACK_POP(&(scpu->stk));

    regIndex = get_inst_reg_id(scpu, curCommand);

    scpu->registers[regIndex] = regNumber;
    return 1;
}

static inline int cpu_cmd_pop(cpu_t* scpu, int* curCommand)
{
    int outputNumber = 0;

    outputNumber = STACK_POP(&(scpu->stk));

    return 1;
}

static inline int cpu_cmd_pushr(cpu_t* scpu, int* curCommand)
{
    int number = 0;
    int regIndex = 0;

    regIndex = get_inst_reg_id(scpu, curCommand);
    number = scpu->registers[regIndex];
    STACK_PUSH(&(scpu->stk), number);

    return 1;
}

static inline int cpu_cmd_in(cpu_t* scpu, int* curCommand)
{
    int inputNumber = 0;

    if (scanf("%d", &inputNumber) != 1)
        return -1;
    STACK_PUSH(&(scpu->stk), inputNumber);

    return 1;
}

static inline int cpu_cmd_out(cpu_t* scpu, int* curCommand)
{
    int outputNumber = 0;

    outputNumber = STACK_POP(&(scpu->stk));
    printf("%d\n", outputNumber);

    return 1;
}

static inline int cpu_cmd_add(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int result = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    result = number1 + number2;
    STACK_PUSH(&(scpu->stk), result);

    return 1;
}

static inline int cpu_cmd_mul(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int result = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    result = number1 * number2;
    STACK_PUSH(&(scpu->stk), result);

    return 1;
}

static inline int cpu_cmd_div(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int result = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    result = number1 / number2;
    STACK_PUSH(&(scpu->stk), result);

    return 1;
}

static inline int cpu_cmd_sub(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int result = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    result = number1 - number2;
    STACK_PUSH(&(scpu->stk), result);

    return 1;
}

static inline int cpu_cmd_sqrt(cpu_t* scpu, int* curCommand)
{
    int number = 0;
    int result = 0;

    number = STACK_POP(&(scpu->stk));

    result = sqrt(number);
    STACK_PUSH(&(scpu->stk), result);

    return 1;
}

static inline int cpu_cmd_ja(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int _command = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    if (number1 > number2) {
        _command = *curCommand;
        *curCommand = scpu->rip->label_com - 1;
        scpu->rip +=  (*curCommand - _command);
    }

    STACK_PUSH(&(scpu->stk), number2);
    STACK_PUSH(&(scpu->stk), number1);

    return 1;
}

static inline int cpu_cmd_jmp(cpu_t* scpu, int* curCommand)
{

    int _command = 0;

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    _command = *curCommand;
    *curCommand = scpu->rip->label_com - 1;
    scpu->rip +=  (*curCommand - _command);

    return 1;
}

static inline int cpu_cmd_jb(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int _command = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    if (number1 < number2) {
        _command = *curCommand;
        *curCommand = scpu->rip->label_com - 1;
        scpu->rip +=  (*curCommand - _command);
    }

    STACK_PUSH(&(scpu->stk), number2);
    STACK_PUSH(&(scpu->stk), number1);

    return 1;
}

static inline int cpu_cmd_jae(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int _command = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    if (number1 >= number2) {
        _command = *curCommand;
        *curCommand = scpu->rip->label_com - 1;
        scpu->rip +=  (*curCommand - _command);
    }

    STACK_PUSH(&(scpu->stk), number2);
    STACK_PUSH(&(scpu->stk), number1);

    return 1;
}

static inline int cpu_cmd_jbe(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int _command = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    if (number1 <= number2) {
        _command = *curCommand;
        *curCommand = scpu->rip->label_com - 1;
        scpu->rip +=  (*curCommand - _command);
    }

    STACK_PUSH(&(scpu->stk), number2);
    STACK_PUSH(&(scpu->stk), number1);

    return 1;
}

static inline int cpu_cmd_je(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int _command = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    if (number1 == number2) {
        _command = *curCommand;
        *curCommand = scpu->rip->label_com - 1;
        scpu->rip +=  (*curCommand - _command);
    }

    STACK_PUSH(&(scpu->stk), number2);
    STACK_PUSH(&(scpu->stk), number1);

    return 1;
}

static inline int cpu_cmd_jne(cpu_t* scpu, int* curCommand)
{
    int number1 = 0;
    int number2 = 0;
    int _command = 0;

    number1 = STACK_POP(&(scpu->stk));
    number2 = STACK_POP(&(scpu->stk));

    (*curCommand)++;

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    if (number1 != number2) {
        _command = *curCommand;
        *curCommand = scpu->rip->label_com - 1;
        scpu->rip +=  (*curCommand - _command);
    }

    STACK_PUSH(&(scpu->stk), number2);
    STACK_PUSH(&(scpu->stk), number1);

    return 1;
}
static inline int cpu_cmd_call(cpu_t* scpu, int* curCommand)
{
    int funcIndex = 0;

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_LABEL_GO)
    {
        return -1;
    }

    STACK_PUSH(&(scpu->stk_ret), *curCommand);
    funcIndex = scpu->rip->label_com;
    scpu->rip +=  (funcIndex - *curCommand - 1);
    *curCommand = funcIndex - 1;

    return 1;
}

static inline int cpu_cmd_ret(cpu_t* scpu, int* curCommand)
{
    int retIndex = 0;

    retIndex = STACK_POP(&(scpu->stk_ret));
    scpu->rip += (retIndex - *curCommand);
    *curCommand = retIndex;

    return 1;
}

static inline int cpu_cmd_pushm(cpu_t* scpu, int* curCommand)
{
    int number = 0;
    int regNumber = 0;
    int regIndex = 0;
    int ramIndex = 0;
    enum ram_operation operation = 0;

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_RAM_BRACKET || scpu->rip->bracket != openBracket)
    {
        printf("Error: expected \'[\'.\n");
        assert(0);
    }

    (*curCommand)++;
    scpu->rip++;

    switch (scpu->rip->tokenType)
    {
        case TOKEN_NUM :
            ramIndex = scpu->rip->number;
            number = scpu->RAM[ramIndex];
            STACK_PUSH(&(scpu->stk), number);

            (*curCommand)++;
            scpu->rip++;

            break;
        case TOKEN_REG :
            regIndex = scpu->rip->reg;
            regNumber = scpu->registers[regIndex];

            (*curCommand)++;
            scpu->rip++;

            if (scpu->rip->tokenType == TOKEN_OP) //plus or minus
            {
                operation = scpu->rip->operation;

                (*curCommand)++;
                scpu->rip++;

                if (scpu->rip->tokenType != TOKEN_NUM)
                {
                    printf("Error: expected number.\n");
                    assert(0);
                }

                number = scpu->rip->number;
                if (operation == OP_PLUS)
                    number = regNumber + number;
                else if (operation == OP_MINUS)
                    number = regNumber - number;
                else
                {
                    printf("Error: unknown operation.\n");
                    assert(0);
                }
                STACK_PUSH(&(scpu->stk), scpu->RAM[number]);

                (*curCommand)++;
                scpu->rip++;
            }
            else {
                STACK_PUSH(&(scpu->stk), scpu->RAM[regNumber]);
            }

            break;
        default :
            printf("Error: expected reg/number.\n");
            assert(0);
    }

    if (scpu->rip->tokenType != TOKEN_RAM_BRACKET || scpu->rip->bracket != closeBracket)
    {
        printf("Error: expected \'[\'.\n");
        assert(0);
    }
    return 1;
}

static inline int cpu_cmd_popm(cpu_t* scpu, int* curCommand)
{
    int number = 0;
    int regNumber = 0;
    int regIndex = 0;
    int ramIndex = 0;
    enum ram_operation operation = 0;

    (*curCommand)++;
    scpu->rip++;

    if (scpu->rip->tokenType != TOKEN_RAM_BRACKET || scpu->rip->bracket != openBracket)
    {
        printf("Error: expected \'[\'.\n");
        assert(0);
    }

    (*curCommand)++;
    scpu->rip++;

    switch (scpu->rip->tokenType)
    {
        case TOKEN_NUM :
            ramIndex = scpu->rip->number;
            number = STACK_POP(&(scpu->stk));
            scpu->RAM[ramIndex] = number;

            (*curCommand)++;
            scpu->rip++;

            break;
        case TOKEN_REG :
            regIndex = scpu->rip->reg;
            regNumber = scpu->registers[regIndex];

            (*curCommand)++;
            scpu->rip++;

            if (scpu->rip->tokenType == TOKEN_OP) //plus or minus
            {
                operation = scpu->rip->operation;

                (*curCommand)++;
                scpu->rip++;

                if (scpu->rip->tokenType != TOKEN_NUM)
                {
                    printf("Error: expected number.\n");
                    assert(0);
                }

                number = scpu->rip->number;
                ramIndex = regNumber + number;
                number = STACK_POP(&(scpu->stk));
                scpu->RAM[ramIndex] = number;

                (*curCommand)++;
                scpu->rip++;
            }
            else
            {
                number = STACK_POP(&(scpu->stk));
                scpu->RAM[regNumber] = number;
            }

            break;
        default :
            printf("Error: expected reg/number.\n");
            assert(0);
    }

    if (scpu->rip->tokenType != TOKEN_RAM_BRACKET || scpu->rip->bracket != closeBracket)
    {
        printf("Error: expected \'[\'.\n");
        assert(0);
    }
    return 1;
}

int get_inst_reg_id(cpu_t* scpu, int* curCommand)
{
    int regIndex = 0;
    (*curCommand)++;
    scpu->rip++;
    if (scpu->rip->tokenType != TOKEN_REG)
    {
        return -1;
    }
    regIndex = scpu->rip->reg;

    return regIndex;
}