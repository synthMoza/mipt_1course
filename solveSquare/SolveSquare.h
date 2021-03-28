#pragma once

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>

// Value to identify infinite amount of roots
#define SS_INF_ROOTS -1
//! Change this to 1 to test function SolveSquare
#define TEST 0
//! The accuracy of calculation
#define EPSILON 0.000001

//----------------------------------------
//! This function scans coefficients for the equation
//! @param a The first coefficient
//! @param b The second coefficient
//! @param c The third coefficient
//----------------------------------------
void inputCoefficients(double* a, double *b, double *c);

//---------------------------------------------------
//! This function solves square equations
//! @param a The first coefficient
//! @param b The second coefficient
//! @param c The third coefficient
//! @param x1 The pointer to the first root
//! @param x2 The pointer to the second root
//! @return The number of roots
//! @note The function returns SS_INF_ROOTS = -1, if there are infinite roots.
//---------------------------------------------------
int SolveSquare(double a, double b, double c, double* x1, double* x2);

//---------------------------------------------------
//! This function solves linear equations
//! @param b The first coefficient
//! @param c The second coefficient
//! @param x1 The pointer to the root
//! @return The number of roots
//----------------------------------------------------
int SolveLine(double b, double c, double* x1);

//-----------------------------------------
//!This function tests SolveSquare
//-----------------------------------------
void SolveSquareTest(void);
