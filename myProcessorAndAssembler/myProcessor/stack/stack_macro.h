#define STACK_SIZE(ptr)                                                                             \
({                                                                                                  \
    typeof(ptr) _ptr = (ptr);                                                                       \
    size_t size = _ptr->size;                                                                       \
    size;                                                                                           \
})

#ifdef SECURE_STACK
#define STACK_PUSH(ptr, arg)                                                                        \
{                                                                                                   \
    if (STACK_OK((ptr)) == 0) {                                                                     \
        stackDump(*(ptr));                                                                          \
        assert(!"OK");                                                                              \
    }                                                                                               \
    if ((ptr)->size >= (ptr)->maxsize)                                                              \
    {                                                                                               \
        (ptr)->data = (Elem_t*) realloc((void*) (ptr)->data, (ptr)->maxsize  + STACK_SIZE_RANGE);   \
        (ptr)->maxsize += STACK_SIZE_RANGE;                                                         \
    }                                                                                               \
    (ptr)->data[(ptr)->size++] = (arg);                                                             \
    (ptr)->hash = hashGenerate(*(ptr));                                                             \
    if (STACK_OK((ptr)) == 0)                                                                       \
    {                                                                                               \
            stackDump(*(ptr));                                                                      \
            assert(!"OK");                                                                          \
    }                                                                                               \
}
#else
#define STACK_PUSH(ptr, arg)                                                                        \
{                                                                                                   \
    if ((ptr)->size >= (ptr)->maxsize)                                                              \
    {                                                                                               \
    (ptr)->data = (Elem_t*) realloc((void*) (ptr)->data, (ptr)->maxsize  + STACK_SIZE_RANGE);       \
    (ptr)->maxsize += STACK_SIZE_RANGE;                                                             \
    }                                                                                               \
    (ptr)->data[(ptr)->size++] = (arg);                                                             \
}
#endif
#ifdef SECURE_STACK
#define STACK_OK(ptr)                                                                               \
({                                                                                                  \
    assert(ptr);                                                                                    \
    int ret = 0;                                                                                    \
    int counter = 0;                                                                                \
    if ((ptr)->size < 0)                                                                            \
    {                                                                                               \
        (ptr)->errno = ER_STACK_SIZE_NEGATIVE;                                                      \
        ret;                                                                                        \
    }                                                                                               \
    if ((ptr)->data == NULL)                                                                        \
    {                                                                                               \
        (ptr)->errno = ER_STACK_DATA_NULLPTR;                                                       \
        ret;                                                                                        \
    }                                                                                               \
    if ((ptr)->hash != hashGenerate(*(ptr)))                                                        \
    {                                                                                               \
        (ptr)->errno = ER_STACK_DIF_HASH;                                                           \
        ret;                                                                                        \
    }                                                                                               \
    if ((ptr)->size > (ptr)->maxsize)                                                               \
    {                                                                                               \
        (ptr)->errno = ER_STACK_SIZE_OVERFLOW;                                                      \
        ret;                                                                                        \
    }                                                                                               \
    for (counter = 0; counter < NUM_OF_CANARIES; counter++) {                                       \
        if ((ptr)->canary1[counter] != STACK_CANARY_VALUE)                                          \
        {                                                                                           \
            (ptr)->errno = ER_STACK_LEFT_CANARY;                                                    \
            ret;                                                                                    \
        }                                                                                           \
        if ((ptr)->canary2[counter] != STACK_CANARY_VALUE)                                          \
        {                                                                                           \
            (ptr)->errno = ER_STACK_RIGHT_CANARY;                                                   \
            ret;                                                                                    \
        }                                                                                           \
    }                                                                                               \
    ret = 1;                                                                                        \
    ret;                                                                                            \
})
#endif
#ifdef SECURE_STACK
#define STACK_POP(ptr)                                                                              \
({                                                                                                  \
    if (STACK_OK((ptr)) == 0) {                                                                     \
        stackDump(*(ptr));                                                                          \
        assert(!"OK");                                                                              \
    }                                                                                               \
    Elem_t Value = 0;                                                                               \
    if ((ptr)->size <= 0)                                                                           \
    {                                                                                               \
        (ptr)->errno = ER_STACK_UNDERFLOW;                                                          \
        Value = 0;                                                                                  \
        Value;                                                                                      \
    }                                                                                               \
    Value = (ptr)->data[--(ptr)->size];                                                             \
    (ptr)->hash = hashGenerate(*(ptr));                                                             \
    if (STACK_OK((ptr)) == 0)                                                                       \
    {                                                                                               \
        stackDump(*(ptr));                                                                          \
        assert(!"OK");                                                                              \
    }                                                                                               \
    Value;                                                                                          \
})
#else
#define STACK_POP(ptr)                                                                              \
({                                                                                                  \
    Elem_t Value = 0;                                                                               \
    Value = (ptr)->data[--(ptr)->size];                                                             \
    Value;                                                                                          \
})
#endif