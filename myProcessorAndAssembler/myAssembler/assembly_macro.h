#define INPUT_FILE_NAME "code.txt"
#define LABEL_MAX_SIZE 25
#define LABEL_MAX_NUMBER 25
#define MAX_VAR_LENGTH 15

#define file_size(file)                                     \
({                                                          \
    size_t size = 0;                                        \
    FILE* _file = (file);                                   \
    fseek(_file, 0, SEEK_END);                              \
    size = ftell(_file);                                    \
    rewind(_file);                                          \
    size;                                                   \
})