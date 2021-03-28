#pragma once

#include "akinator_macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

//! The name of the file with the base of answers for Akinator
#define BASE_FILE_NAME "base.txt"
typedef struct Node tree;

//! Analazies the tree, creates the buffer of the input file and builds the tree of it
//! @param buffer The buffer with all text from the file
//! @return The head of the final tree
tree* analyzeTree(char** buffer);
//! Builds a tree using the buffer
//! @param buffer The buffer with all text from the file
//! @param fileSize The size of the input file
//! @return The final tree
tree* buildTree(char** buffer, int* fileSize);
//! Outputs the tree into the file using certain format
//! @param input_tree The tree to be written into the file
//! @param output_file The pointer to the output file
void outputTree(tree* input_tree, FILE* output_file);
//! Playing Akinator - asks questions and guess the person you thought
//! @param input_tree The tree with the questions and answers
void akinatorAsk(tree* input_tree);
//! Playing Akinator - add another answer and the question from the input for Akinator
//! @param input_tree The tree with the questions
void addAnotherOption(tree* input_tree);
//! Gets the full line from the input withput '\n'm allocatesmemory for it
//! @return The pointer to the string
char* getLine(void);
//! Free the whole tree
//! @param input_tree The tree to be cleared
void freeNode(tree* input_tree);