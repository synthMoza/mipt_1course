#include "akinator.h"
#include "stack.h"

//! The structure of the tree
//! @param leftChild The left node of the tree
//! @param rightChild The right node of the tree
//! @param value The value of the node
struct Node {
    struct Node* leftChild; //left node of the tree
    char* value; //input string
    struct Node* rightChild;//the right one
};

//The structure of pairs (statement+plus pos/neg answer sign) for pinpoiting the location of the element
//! @param value The value of the node
//! @param sign  boolean sign of the no/yes answer
struct Pair {
    char* value;
    bool sign;
};


int main() {
    tree* input_tree = NULL;
    FILE* output_file = NULL;
    char* answer = NULL;
    char* buffer = NULL;
    int game_answer = 0;
    int fileSize = 0;
    stack_t stk;
    stack_t path;

    stackConstruct(&stk);
    stackConstruct(&path);
    input_tree = buildTree(&buffer, &fileSize);

    AKINATOR_START_DIALOGUE;
    sleep(1);
    WAITING_FOR_THE_ANSWER;
    scanf("%d", &game_answer);
    getchar();
    sleep(1);
    if (game_answer == 2) {
        //SECOND STEP - ACTIVATING THE MAIN GAME
        WELCOME_AKINATOR;
        sleep(1);
        WAITING_FOR_THE_ANSWER;
        answer = getLine();

        sleep(1);
        if (strcmp(answer, "да") == 0) {
            AKINATOR_PLAY;
        } else if (strcmp(answer, "нет") == 0) {
            AKINATOR_DECIDE_NOT_TO_PLAY;
        } else {
            AKINATOR_TROUBLED;
        }
        sleep(1);
        free(answer);
        AKINATOR_START_GAME;

        akinatorAsk(input_tree);

        output_file = fopen(BASE_FILE_NAME, "w");

        outputTree(input_tree, output_file);

        fclose(output_file);

        free(buffer - fileSize);
        freeNode(input_tree);
    } else if (game_answer == 1) {
        char* purpose = NULL;
        AKINATOR_ASKS_A_CHARACTER;
        sleep(1);
        WAITING_FOR_THE_ANSWER;
        purpose = getLine();
        sleep(1);
        pinpointElement(input_tree, purpose, &stk, &path);
        free(purpose);
    }
    return 0;
}

tree* buildTree(char** buffer, int* fileSize)
{
    FILE* input_file = NULL;
    tree* input_tree = NULL;

    input_file = fopen(BASE_FILE_NAME, "r");
    *fileSize = FILE_SIZE(input_file);
    *buffer = (char*) calloc (*fileSize, sizeof(char));
    fread(*buffer, sizeof(char), *fileSize, input_file);

    if (**buffer == '(') //the head of the tree that we need
    {
        (*buffer)++;
        input_tree = analyzeTree(buffer);
    }

    fclose(input_file);

    return input_tree;
}

tree* analyzeTree(char** buffer)
{
    tree* input_tree = NULL;

    input_tree = (tree*) calloc (1, sizeof(tree));

    while (**buffer != '\0')
    {
        if (**buffer == '(')
        {
            (*buffer)++;
            input_tree->leftChild = analyzeTree(buffer);

        }
        else if (**buffer == '*') //empty node
        {
            input_tree->leftChild = NULL;
            (*buffer)++;
        }

        if (**buffer == ',') //heading for the value of the node
        {
            (*buffer)++;
        }

        if (**buffer == '\"') //wring the value
        {
            (*buffer)++;

            input_tree->value = *buffer;

            while (**buffer != '\"')
                (*buffer)++;
            **buffer = '\0';
            (*buffer)++;
        }
        else if (**buffer == '0')
        {
            input_tree->value = NULL;
            (*buffer)++;
        }

        if (**buffer == ',') //heading for the right tree
            (*buffer)++;

        if (**buffer == '(') //the right tree itself
        {
            (*buffer)++;
            input_tree->rightChild = analyzeTree(buffer);
        }
        else if (**buffer == '*') //empty node
        {
            input_tree->rightChild = NULL;
            (*buffer)++;
        }

        if (**buffer == ')') //everything's good
            (*buffer)++;

        return input_tree;
    }

    return input_tree;
}

void outputTree(tree* input_tree, FILE* output_file)
{
    if (input_tree == NULL)
    {
        fprintf(output_file, "*");
    } else {
        fprintf(output_file, "(");
        outputTree(input_tree->leftChild, output_file);
        fprintf(output_file, ",\"%s\",", input_tree->value);
        outputTree(input_tree->rightChild, output_file);
        fprintf(output_file, ")");
    }
}

void akinatorAsk(tree* input_tree)
{
    char* answer = NULL;

    if ((input_tree->leftChild != NULL) && (input_tree->rightChild != NULL)) //let the game start
    {
        sleep(1);
        AKINATOR_ASK
        printf("Ваш персонаж %s?\n", input_tree->value);
        WAITING_FOR_THE_ANSWER
        answer = getLine();
        if (strcmp(answer, "да") == 0) {
            akinatorAsk(input_tree->leftChild);
        }
        else if (strcmp(answer, "нет") == 0)
        {
            akinatorAsk(input_tree->rightChild);
        } else
        {
            AKINATOR_DEATH;
            assert(!"DIE!");
        }
        free(answer);
    }
    else
    {
        sleep(1);
        AKINATOR_THINKING
        sleep(1);
        AKINATOR_GUESS(input_tree->value)
        WAITING_FOR_THE_ANSWER
        answer = getLine();

        sleep(1);
        if (strcmp("да", answer) == 0) {
            AKINATOR_CORRECT_GUESS

        }
        else if (strcmp("нет", answer) == 0)
        {
            AKINATOR_WRONG_GUESS
            addAnotherOption(input_tree);
        }
        else
        {
            AKINATOR_DEATH;
            assert(!"DIE!");
        }
        free(answer);
    }
}

void addAnotherOption(tree* input_tree)
{
    char* question = NULL;
    char* name = NULL;

    sleep(1);
    AKINATOR_ADDING_NEW_VALUE;
    sleep(1);
    WAITING_FOR_THE_ANSWER;
    name = getLine();

    AKINATOR_NEW_VALUE_QUESTION
    WAITING_FOR_THE_ANSWER;
    question = getLine();

    input_tree->leftChild = (tree*) calloc (1, sizeof(tree));
    input_tree->rightChild = (tree*) calloc(1, sizeof(tree));

    input_tree->rightChild->value = input_tree->value;
    input_tree->leftChild->value = name;

    input_tree->value = question; //adding the question

    AKINATOR_BECAME_STRONGER;
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

void freeNode(tree* input_tree)
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

void printPath(stack_t* path)
{
    struct Pair* pair = NULL;
    stack_t _stk;
    stackConstruct(&_stk);
    printf("Выбранный персонаж ");

    while (path->size != 1)
    {
        pair = STACK_POP(path);
        STACK_PUSH(&_stk, pair);
        if (pair->sign == 1)
            printf("%s, ", pair->value);
        else
            printf("не %s, ", pair->value);
    }
    pair = STACK_POP(path);
    STACK_PUSH(&_stk, pair);
    if (pair->sign == 1)
        printf("%s.\n", pair->value);
    else
        printf("не %s.\n", pair->value);
    while (_stk.size > 0)
    {
        STACK_PUSH(path, STACK_POP(&_stk));
    }
    stackDestroy(&_stk);
}

void pinpointElement(tree* input_tree, char* name, stack_t* stk, stack_t* path) {
    if (input_tree->leftChild != NULL) {
        struct Pair* pair = (struct Pair*) calloc(1, sizeof(struct Pair));
        pair->value = input_tree->value;
        pair->sign = 1;
        STACK_PUSH(stk, pair);
        pinpointElement(input_tree->leftChild, name, stk, path);
        STACK_POP(stk);
    }
    if (input_tree->rightChild != NULL) {
        struct Pair* pair = (struct Pair*) calloc(1, sizeof(struct Pair));
        pair->value = input_tree->value;
        pair->sign = 0;
        STACK_PUSH(stk, pair);
        pinpointElement(input_tree->rightChild, name, stk, path);
        STACK_POP(stk);
    }
    if (strcmp(name, input_tree->value) == 0) {
        int counter = 0;
        printPath(stk);
        return ;
    }
}