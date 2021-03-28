#include "list.h"

void listConstruct(list_t* roster)
{
    LIST_CALLOC(roster->data)
    LIST_CALLOC(roster->prev)
    LIST_CALLOC(roster->next)
    LIST_CALLOC(roster->freeBox)

    roster->head = EMPTY_LIST_SIGN;
    roster->tail = EMPTY_LIST_SIGN;

    roster->size = 0;
    roster->maxSize = LIST_SIZE;
}

void listPrint(list_t roster)
{
    int counter = 0;
    int right_index = 1;
    if (roster.head == -1)
    {
        printf("EMPTY LIST\n");
        return ;
    }
    counter = roster.head;
    while (roster.next[counter] != 0)
    {
        printf("list[%d] = %d\n", right_index, roster.data[counter]);
        counter = roster.next[counter];
        right_index++;
    }
    printf("list[%d] = %d\n", right_index, roster.data[counter]);
}

int listPinpointIndex(list_t roster, int value)
{
    int counter = 0;

    for (counter = 1; counter <= roster.maxSize; counter++)
    {
        if (roster.data[counter] == value)
        {
            return counter;
        }
    }

    return -1;
}
Elem_t listPinpointValue(list_t roster, int index)
{
    assert(index > 0);

    return (roster.freeBox[index] == OCCUPIED_UNIT_SIGN) ? roster.data[index] : -1;
}

void listDestroy(list_t* roster)
{
    assert(roster);

    free(roster->data);
    free(roster->freeBox);
    free(roster->next);
    free(roster->prev);
}

void listClear(list_t* roster)
{
    assert(roster);

    int counter = 0;

    for(counter = 0; counter <= roster->maxSize; counter++)
    {
        roster->data[counter] = 0;
        roster->prev[counter] = 0;
        roster->next[counter] = 0;
        roster->freeBox[counter] = FREE_UNIT_SIGN;
    }

    roster->size = 0;
    roster->head = -1;
    roster->tail = -1;
}

int list_is_valid(list_t* roster)
{
    assert(roster);

    if (LIST_FIND_TAIL(*roster) != roster->tail)
    {
        roster->errno = ER_LIST_DIF_TAIL;
        return -1;
    }
    if (LIST_FIND_HEAD(*roster) != roster->tail)
    {
        roster->errno = ER_LIST_DIF_HEAD;
        return -1;
    }
    if (roster->data == NULL || roster->freeBox == NULL || roster->next == NULL || roster->prev == NULL)
    {
        roster->errno = ER_LIST_NULLPTR;
        return -1;
    }
    if (roster->size < 0 || roster->size > roster->maxSize)
    {
        roster->errno = ER_LIST_SIZE_WRONG;
        return -1;
    }
    if (roster->size != LIST_SIZE_COUNT(*roster))
    {
        roster->errno = ER_LIST_SIZE_DIF;
        return -1;
    }
    if (roster->data[0] != 0 || roster->freeBox[0] != 0 || roster->next[0] != 0 || roster->prev[0] != 0)
    {
        roster->errno = ER_LIST_POISON_ELEMENT;
        return -1;
    }
    return 0;
}

void listDump(list_t* roster)
{
    int counter = 0;
    assert(roster);

    FILE* output_file = freopen(LOG_FILE_NAME, "w", stdout);

    printf("list \"%s\" [%x] ", roster->name, roster);
    (roster->errno == 0) ? printf("(ok)\n") : printf("(error)\n");
    printf("{\n");
    printf("\t data = [%x]", roster->data);
    printf("\t{\n");
    for (counter = 0; counter < roster->maxSize;counter++)
    {
        printf("\t\tdata[%d] = %d\n", counter, roster->data[counter]);
    }
    printf("\t}\n");
    printf("\t next = [%x]", roster->next);
    printf("\t{\n");
    for (counter = 0; counter < roster->maxSize;counter++)
    {
        printf("\t\tnext[%d] = %d\n", counter, roster->next[counter]);
    }
    printf("\t}\n");
    printf("\t prev = [%x]", roster->prev);
    printf("\t{\n");
    for (counter = 0; counter < roster->maxSize;counter++)
    {
        printf("\t\tprev[%d] = %d\n", counter, roster->prev[counter]);
    }
    printf("\t}\n");
    printf("\t freeBox = [%x]", roster->freeBox);
    printf("\t{\n");
    for (counter = 0; counter < roster->maxSize;counter++)
    {
        printf("\t\tfreeBox[%d] = %d\n", counter, roster->freeBox[counter]);
    }
    printf("\t}\n");
    printf("\tsize = %d\n", roster->size);
    printf("\tmaxSize = %d\n", roster->maxSize);
    printf("\thead = %d\n", roster->head);
    printf("\ttail = %d\n", roster->tail);
    printf("\terrno = %d\n", roster->errno);
    printf("}\n");

    fclose(output_file);
}

