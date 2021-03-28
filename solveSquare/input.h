#pragma once

#include <stdio.h>
#include <assert.h>
#include <ctype.h>

//----------------------------------------------
//! The function returns the input double number
//! @param num_sign The sign, that the input string was a number
//! @return The number
//! @note If the input string isn't a number, puts "-1" into num_sign and returns "-1";
//----------------------------------------------
double getNum(int* num_sign);