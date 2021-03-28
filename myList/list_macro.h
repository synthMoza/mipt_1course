#define LIST_SIZE 2

//! Simple macro for allocating memory for list's component
#define LIST_CALLOC(ptr)                                                        \
{                                                                               \
    (ptr) = (Elem_t*) calloc(LIST_SIZE, sizeof(Elem_t));                        \
}
#define LIST_REALLOC(ptr)                                                       \
{                                                                               \
    (ptr)->data = (Elem_t*) realloc((ptr)->data, (ptr)->size + LIST_SIZE);      \
    (ptr)->prev = (Elem_t*) realloc((ptr)->prev, (ptr)->size + LIST_SIZE);      \
    (ptr)->next = (Elem_t*) realloc((ptr)->next, (ptr)->size + LIST_SIZE);      \
    (ptr)->freeBox = (Elem_t*) realloc((ptr)->freeBox, (ptr)->size + LIST_SIZE);\
}
//! Inserts the element into the end of the list
//! @param ptr The pointer to the list
//! @param arg The element
#define LIST_INSERT_LAST(ptr, arg)                                              \
{                                                                               \
    assert(ptr);                                                                \
    int free_index = -1;                                                        \
    free_index = LIST_PINPOINT_FREE_INDEX((ptr));                               \
    (ptr)->data[free_index] = (arg);                                            \
    (ptr)->size ++;                                                             \
    if ((ptr)->head == -1)                                                      \
    {                                                                           \
        (ptr)->head = free_index;                                               \
        (ptr)->tail = free_index;                                               \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        (ptr)->prev[free_index] = (ptr)->tail;                                  \
        (ptr)->next[(ptr)->tail] = free_index;                                  \
        (ptr)->tail = free_index;                                               \
    }                                                                           \
}

#define LIST_INSERT_FIRST(ptr, arg)                                             \
{                                                                               \
    assert(ptr);                                                                \
    int free_index = -1;                                                        \
    free_index = LIST_PINPOINT_FREE_INDEX((ptr));                               \
    (ptr)->data[free_index] = (arg);                                            \
    (ptr)->size ++;                                                             \
    if ((ptr)->head == -1)                                                      \
    {                                                                           \
        (ptr)->head = free_index;                                               \
        (ptr)->tail = free_index;                                               \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        (ptr)->next[free_index] = (ptr)->head;                                  \
        (ptr)->prev[(ptr)->head] = free_index;                                  \
        (ptr)->head = free_index;                                               \
    }                                                                           \
}

#define LIST_DELETE(ptr, index)                                                 \
{                                                                               \
    assert(index > 0);                                                          \
    assert(ptr);                                                                \
    (ptr)->next[(ptr)->prev[index]] = (ptr)->next[index];                       \
    (ptr)->prev[(ptr)->next[index]] = (ptr)->prev[index];                       \
    (ptr)->freeBox[index] = FREE_UNIT_SIGN;                                     \
    (ptr)->size --;                                                             \
}

#define LIST_DELETE_FIRST(ptr)                                                  \
{                                                                               \
    assert(ptr);                                                                \
    (ptr)->freeBox[(ptr)->head] = FREE_UNIT_SIGN;                               \
    if ((ptr)->head != (ptr)->tail)                                             \
    {                                                                           \
        (ptr)->head = (ptr)->next[(ptr)->head];                                 \
        (ptr)->prev[(ptr)->head] = 0;                                           \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        (ptr)->head = -1;                                                       \
        (ptr)->tail = -1;                                                       \
    }                                                                           \
    (ptr)->size --;                                                             \
}

#define LIST_DELETE_LAST(ptr)                                                   \
{                                                                               \
    assert(ptr);                                                                \
    (ptr)->freeBox[(ptr)->tail] = FREE_UNIT_SIGN;                               \
    if ((ptr)->head != (ptr)->tail)                                             \
    {                                                                           \
        (ptr)->tail = (ptr)->prev[(ptr)->tail];                                 \
        (ptr)->next[(ptr)->tail] = 0;                                           \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        (ptr)->head = -1;                                                       \
        (ptr)->tail = -1;                                                       \
    }                                                                           \
    (ptr)->size --;                                                             \
}

#define LIST_INSERT_BEFORE(ptr, index, arg)                                     \
{                                                                               \
    assert((ptr));                                                              \
    assert((index) > 0);                                                        \
    if ((index) == (ptr)->head)                                                 \
    {                                                                           \
        LIST_INSERT_FIRST((ptr), (arg));                                        \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        int free_index = -1;                                                    \
        free_index = LIST_PINPOINT_FREE_INDEX((ptr));                           \
        (ptr)->data[free_index] = (arg);                                        \
        (ptr)->size ++;                                                         \
        (ptr)->prev[free_index] = (ptr)->prev[(index)];                         \
        (ptr)->next[free_index] = (index);                                      \
        (ptr)->next[(ptr)->prev[(index)]] = free_index;                         \
        (ptr)->prev[(index)] = free_index;                                      \
    }                                                                           \
}

#define LIST_INSERT_AFTER(ptr, index, arg)                                      \
{                                                                               \
    assert((ptr));                                                              \
    printf("WTF ASSERT %d\n", index);                                          \
    assert((index) > 0);                                                        \
    if ((index) == (ptr)->tail)                                                 \
    {                                                                           \
        LIST_INSERT_LAST((ptr), (arg));                                         \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        LIST_INSERT_BEFORE((ptr), (ptr)->next[(index)], (arg));                 \
    }                                                                           \
}

#define LIST_FIND_TAIL(list)                                                    \
({                                                                              \
    int counter = 1;                                                            \
    for (counter = 1; (list).next[(list).data[counter]] != 0 && (list).freeBox[counter] == 1; counter++) ; \
    counter;                                                                    \
})

#define LIST_FIND_HEAD(list)                                                    \
({                                                                              \
    int index = 0;                                                              \
    int counter = 1;                                                            \
    for (counter = 1; (list).prev[(list).data[counter]] != 0 && (list).freeBox[counter] == 1; counter++) ;      \
    counter;                                                                    \
})

#define LIST_GET_PTR_FIRST(list) &(list).data[(list).head]
#define LIST_GET_PTR_LAST(list) &(list).data[(list).tail]
#define LIST_GET_PTR_BEFORE(list, index) &(list).data[(list).prev[(index)]]
#define LIST_GET_PTR_AFTER(list, index) &(list).data[(list).next[(index)]]

#define LIST_SIZE_COUNT(list)                                                   \
({                                                                              \
    int size = 0;                                                               \
    int counter = 0;                                                            \
    for (counter = 1; counter <= (list).maxSize; counter++)                     \
    {                                                                           \
        if ((list).freeBox[counter] == 1)                                      \
            size++;                                                             \
    }                                                                           \
    size;                                                                       \
})

#define LIST_PINPOINT_FREE_INDEX(ptr)                                           \
({                                                                              \
    int counter = 0;                                                            \
    int index_free = -1;                                                        \
    do                                                                          \
    {                                                                           \
        for (counter = 1; counter < (ptr)->maxSize; counter++)                  \
        {                                                                       \
            if ((ptr)->freeBox[counter] ==  FREE_UNIT_SIGN)                     \
            {                                                                   \
                (ptr)->freeBox[counter] = OCCUPIED_UNIT_SIGN;                   \
                index_free = counter;                                           \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        if (index_free == -1)                                                   \
        {                                                                       \
            LIST_REALLOC((ptr))                                                 \
            (ptr)->maxSize += LIST_SIZE;                                        \
        }                                                                       \
    } while (index_free == -1);                                                 \
    index_free;                                                                 \
})