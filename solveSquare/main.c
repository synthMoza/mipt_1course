#include "SolveSquare.h"

int main() {
    if (TEST)
        SolveSquareTest(); //test of Solve Square
    int nRoots = 0;
    double x1 = 0, x2 = 0;
    double a = 0, b = 0, c = 0;
    inputCoefficients(&a, &b, &c); //enter coefficients
    nRoots = SolveSquare(a, b, c, &x1, &x2); //check the number of roots, calculate them
    switch (nRoots) { //shows the roots if they exist
        case 0 :
            printf("NO ROOTS\n");
            break;
        case 1 :
            printf("x = %g\n", x1);
            break;
        case 2 :
            printf("x1 = %g, x2 = %g\n", x1, x2);
            break;
        case SS_INF_ROOTS :
            printf("inf roots\n");
            break;
        default :
            printf("main(): error\n");
    }
    return 0;
}