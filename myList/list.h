#include "list_macro.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY_LIST_SIGN -1
#define OCCUPIED_UNIT_SIGN 1
#define FREE_UNIT_SIGN 0
#define LOG_FILE_NAME "list_log.txt"
#define LIST_NAME_SIZE 20

enum errors{ER_LIST_DIF_TAIL, ER_LIST_DIF_HEAD, ER_LIST_NULLPTR, ER_LIST_SIZE_WRONG, ER_LIST_SIZE_DIF, ER_LIST_POISON_ELEMENT
};

typedef int Elem_t;

//! Structure of list
//! @param name The name of the variable
//! @param data The data array of the list
//! @param prev The array of the previous indexes of the elements
//! @param next The array of the next indexes of the elements
//! @param head The "head" index of the list
//! @param size The current size of the list
//! @param maxSize The maximum size of the list
//! @param freeBox The array that signs if the unit is free
//! @param errno The number of the error occured(0 if there are no errors)
struct list {
    char name[LIST_NAME_SIZE];
    Elem_t* data;
    int* prev;
    int* next;
    int head;
    int tail;
    int size;
    int maxSize;
    int* freeBox;
    int errno;
};

typedef struct list list_t;
//!The constructor of the list. Allocates memory and "zeroes" elements
//! @param roster The list
void listConstruct(list_t* roster);
//!Prints the elements of the list in the corret order
//! @param roster The list
void listPrint(list_t roster);
//! Pinpoints the index by the given value
//! @param roster The list
//! @param value The necessary value
//! @return The index if the element was found, -1 if not.
int listPinpointIndex(list_t roster, int value);
//! Clears the whole list
//! @param roster The list
void listClear(list_t* roster);
//!Destroys the whole list
//! @param roster The list
void listDestroy(list_t* roster);
//! Pinpoints the value by its index
//! @param roster The list
//! @param index The index of the value
//! @return The value if this index exists, -1 if not
Elem_t listPinpointValue(list_t roster, int index);
//! Some tests for the list to check is it is valid
//! @param roster The list
//! @param -1,if there is some error, 0 if not
int list_is_valid(list_t* roster);
//!Dump for the list with all variables
//! @param roster The list
void listDump(list_t* roster);

