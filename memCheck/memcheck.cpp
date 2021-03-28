#include <iostream>

//! The name of the output log file
#define LOG_FILE_NAME "memory.log"

//!Usage of macros:
//!<variable> = new <type>; => <variable> = NEW <type>
#define NEW new(__FILE__, __PRETTY_FUNCTION__, __LINE__)
//!delete(<variable>) => DELETE(<variable>)
#define DELETE(memory) operator delete (memory, __FILE__, __PRETTY_FUNCTION__, __LINE__)
//! delete[](<variable>) => DELETE_(<variable>)
#define DELETE_(memory) operator delete[](memory, __FILE__, __PRETTY_FUNCTION__, __LINE__)

//!Log file pointer
FILE* log_file = nullptr;

void* operator new(size_t size, const std::string& file_name, const std::string& func_name, int com_line)
{
    void* memory = nullptr;

    memory = ::operator new(size);

    fprintf(log_file, "[%p] Memory allocating with \"new\": file "
                                  "\"%s\", function \"%s\", line %d, size %zu.\n", memory, file_name.c_str(), func_name.c_str(), com_line, size);
    return memory;
}

void * operator new[](size_t size, const std::string& file_name, const std::string& func_name, int com_line)
{
    void* memory = nullptr;

    memory = ::operator new[](size);

    fprintf(log_file, "[%p] Memory allocating with \"new[]\": file "
                      "\"%s\", function \"%s\", line %d, size %zu.\n", memory, file_name.c_str(), func_name.c_str(), com_line, size);
    return memory;
}

void operator delete(void* memory, const std::string& file_name, const std::string& func_name, int com_line)
{
    fprintf(log_file, "[%p] Memory free-up with \"delete\": file "
                      "\"%s\", function \"%s\", line %d.\n", memory, file_name.c_str(), func_name.c_str(), com_line);
    ::operator delete(memory);
}

void operator delete[](void* memory, const std::string& file_name, const std::string& func_name, int com_line)
{
    fprintf(log_file, "[%p] Memory free-up with \"delete\": file "
                      "\"%s\", function \"%s\", line %d.\n", memory, file_name.c_str(), func_name.c_str(), com_line);
    ::operator delete[](memory);
}

//! Initializes the log file (file name - macro LOG_FILE_NAME)
//! This functions must be called at the beginning of "int main()"
void init_log()
{
    log_file = fopen(LOG_FILE_NAME, "w");
}
//! Closes the log file
//! This function must be called at the end of "int main()"
void close_log()
{
    fclose(log_file);
}