#define INPUT_FILE_NAME "/home/synthmoza/CLionProjects/myAssembler/cmake-build-debug/code.bin"
#define CPU_RAM_SIZE 200

#define file_size(file)                                     \
({                                                          \
    size_t size = 0;                                        \
    FILE* _file = (file);                                   \
    fseek(_file, 0, SEEK_END);                              \
    size = ftell(_file);                                    \
    rewind(_file);                                          \
    size;                                                   \
})

#define cpu_cmd_end(p1, p2) 2;