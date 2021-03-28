#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

//! The name of the input file
#define INPUT "hamlet.txt"
//! The name of the output file
#define OUTPUT "output_hamlet.txt"

//! The structure to represent string
//! @param value The pointer to the string
//! @param length The length of the string
typedef struct string {
    char* value;
    int length;
} string;

//! Counts the size of the file
//! @param bufsize The size of the buffer(text)
void countSize(int* bufsize);

//! Fills the buf from the file
//! @param bufsize The size of the buffer
//! @param buf The buf
void fillBuf(char* buf, int bufsize);

//! This function counts the number of symbols and lines
//! @param buf The buffer
//! @param nlines NUmber of lines
//! @param bufsize The size of buffer
void countLines(char* buf, int* nlines, int bufsize);

//! This function fills the string structed buf
//! @param nlines number of lines
//! @param buf the buf
//! @param bufsize THe size of the buf
//! @param key The array of strings
void fillStringStructWithBuf(char* buf, int nlines, struct string* key[], int bufsize);

//! This function sorts forwards and backwards and outputs into the file array of strings
//! @param key The array of strings
//! @param nlines Number of lines
void sortThemAndOutputThem(struct string* key[], int nlines);

//! Clears the memory after the whole program ran
//! @param key The array of strings
//! @param nlines The number of lines
//! @param buf The buffer
void clearMemory(string* key[], char* buf, int nlines);