#include "input.h"

//GETCH AND UNGETCH BUF AND MORE DECLARATION

//! The size of getch() buffer
#define BUFSIZE 100
//! The buffer of getch()
int buf[BUFSIZE];
//! The index of the current element in the buffer
int buf_num = 0;

//-----------------------------------------
//! Getch works like getchar(), but has its own buffer
//! @return returns the last elements from the buffer(if it exists), or the input symbol
//-----------------------------------------
static int getch(void) {
    return (buf_num > 0) ? buf[buf_num--] : getchar();
}
//-----------------------------------------
//! Works with the pair of getch(). Puts the symbol into the buffer, if there is enough space
//! @param symbol The symbol
//-----------------------------------------
static void ungetch(int symbol) {
    if (buf_num >= BUFSIZE) {
        printf("ERROR: NOT ENOUGH SPACE IN BUFFER\n");
        assert(0);
    }
    else {
        buf[++buf_num] = symbol;
    }

}
//-----------------------------------------
//! Clears the buffer of getch() / ungetch()
//-----------------------------------------
static void clear_getch(void) {
    buf_num = 0;
}

double getNum(int* num_sign) {
    assert(num_sign != NULL);

    int symbol = 0;
    int result = 0;
    int power = 1;
    int sign = 1;
    while(isspace(symbol = getch())) ;
    if (symbol == '-') {
        sign = -1;
        symbol = getch();
    }
    ungetch(symbol);
    while (isdigit(symbol = getch())) {
            result = result * 10 + (symbol - '0');
    }
    if (isalpha(symbol)) {
        while((symbol = getch()) != ' ' && symbol == '\n') ;
        *num_sign = -1;
        return -1;

    }
    if (symbol == '.') {
        while (isdigit(symbol = getch())) {
            result = result * 10 + (symbol - '0');
            power = power * 10;
        }
    }
    *num_sign = 1;
    ungetch(symbol);
    return result * sign / power;
}