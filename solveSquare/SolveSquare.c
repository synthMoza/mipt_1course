#include "SolveSquare.h"
#include "input.h"

//----------------------------------------
//! This function compares two numbers of type double with EPSILON accuracy
//! @param a The first number
//! @param b The second number
//! @param EPS Accuracy
//! @return 1 if are equal with said accuracy, 0 if not
//----------------------------------------
static int isequal(double a, double b, double EPS) {
    assert(isfinite(a));
    assert(isfinite(b));

    return (fabs(a-b) < EPS) ? 1 : 0;
}
//----------------------------------------
//! This function compares the given number of type double and zero with EPSILON accuracy
//! @param a The given number
//! @param EPS Accuracy
//! @return 1 if more than zero with said accuracy, 0 if not
//----------------------------------------
static int is_more_zero(double a, double EPS) {
    assert(isfinite(a));

    return (a > EPS || a > -EPS) ? 1 : 0;
}
//----------------------------------------
//! This function compares the number of type double and zero with EPSILON accuracy
//! @param a The first number
//! @param b The second number
//! @param EPS Accuracy
//! @return 1 if the given number equals zero with said accuracy, 0 if not
//----------------------------------------
static int is_zero(double a, double EPS) {
    assert(isfinite(a));

    return (fabs(a) < EPS) ? 1 : 0;
}

void inputCoefficients(double* a, double *b, double *c) {
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(a != b);
    assert(b != c);
    assert(a != c);

    int num_sign_a = 1;
    int num_sign_b = 1;
    int num_sign_c = 1;

    printf("Enter a, b, c >>> ");
    *a = getNum(&num_sign_a);
    *b = getNum(&num_sign_b);
    *c = getNum(&num_sign_c);
    while (num_sign_a == -1 || num_sign_b == -1 || num_sign_c == -1) {
        printf("ERROR: Wrong Input. Enter coefficients again >>> ");
        *a = getNum(&num_sign_a);
        *b = getNum(&num_sign_b);
        *c = getNum(&num_sign_c);
    }
}

int SolveSquare(double a, double b, double c, double *x1, double *x2) {
    double d = 0.0, sqrt_d = 0.0;
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    assert(x1 != NULL);
    assert(x1 != NULL);
    assert(x1 != x2);
    if (is_zero(a, EPSILON))
        return SolveLine(b, c, x1);
    else { /*a != 0*/
        d = b * b - 4.0 * a * c;

        assert(isfinite(d));
        if (is_zero(d, EPSILON)) {
            *x1 = (-b) / (a*2);
            assert(isfinite(*x1));

            return 1;
        }
        else if (is_more_zero(d, EPSILON)) {
            sqrt_d = sqrt(d);
            *x1 = (-b + sqrt_d) / (2*a);
            *x2 = (-b - sqrt_d) / (2*a);

            assert(isfinite(*x1));
            assert(isfinite(*x2));
            return 2;
        }
        else
            return 0;
    }
}

int SolveLine(double b, double c, double *x1) {
    assert(isfinite(b));
    assert(isfinite(c));
    assert(x1 != NULL);

    if (isequal(b, 0, EPSILON)) {
        if (isequal(c, 0, EPSILON)) {
            return SS_INF_ROOTS;
        }
        else { /*c != 0*/
            return 0;
        }
    }
    else { /*b != 0*/
        *x1 = -c/b;

        assert(isfinite(*x1));
        return 1;
    }
}

void SolveSquareTest(void) {
    double x1, x2;
    if (SolveSquare(0, 0, 0, &x1, &x2)!= SS_INF_ROOTS) {
        printf("TEST 1 FAILED (a = 0, b = 0, c = 0\n)");
    }
    else
        printf("TEST 1 OK (a = 0, b = 0, c = 0)\n");
    if (SolveSquare(0, 0 , 1, &x1, &x2) != 0)
        printf("TEST 2 FAILED (a = 0, b = 0, c = 1)\n");
    else
        printf("TEST 2 OK (a = 0, b = 0, c = 1)\n");
    if (SolveSquare(0, 2, 2, &x1, &x2) != 1 && x1 != -1)
        printf("TEST 3 FAILED (a = 2, b = 2, c = 2)\n");
    else
        printf("TEST 3 OK (a = 0, b = 2, c = 2)\n");
    if (SolveSquare(1, 2, 1, &x1, &x2) != 1 && x1 != -1)
        printf("TEST 4 FAILED (a = 1, b = 2, c = 1)\n");
    else
        printf("TEST 4 OK (a = 1, b = 2, c = 1)\n");
    if (SolveSquare(1, 2, -3, &x1, &x2) != 2 && x1 != 1 && x2 != -3)
        printf("TEST 5 FAILED (a = 1, b = 2, c = -3)\n");
    else
        printf("TEST 5 OK (a = 1, b = 2, c = -3)\n");
}