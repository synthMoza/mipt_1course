#include <iostream>

#define START_SIZE 10
#define POISON_SIZE -13

#define MIN(a, b) ((a) > (b)) ? (b) : (a)

#define ENUM_CATCH_ERROR                                                                        \
catch (ENUM_ERROR_CODES CODE)                                                                   \
{                                                                                               \
    std::cerr << "Error " << CODE << " occured in " << __PRETTY_FUNCTION__ << "." << std::endl; \
    abort();                                                                                    \
}                                                                                               \


//! List of errors by their codes:
//! 0 - ENUM_INVALID_INDEX - Invalid index for this vector (less than zero or greater than the capacity)
//! 1 - ENUM_EMPTY_VECTOR - Trying to pull from the empty vector
enum ENUM_ERROR_CODES
{
    ENUM_INVALID_INDEX, ENUM_EMPTY_VECTOR
};
