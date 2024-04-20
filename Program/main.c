#include <stdio.h>
#include <stdlib.h>

typedef double (*func_t)(double);



double chordIteration(func_t F, double a, double b, double x) {
    if ((F(b) - F(a)) * ((b - a) / 2 - F((b - a) / 2)) > 0){
        return (x - F(x) * (b - x) / (F(b) - F(x)));
    } else {
        return (x - F(x) * (a - x) / (F(a) - F(x)));
    }
}

double tangentIteration(func_t F, func_t F1, double a, double b, double x) {
    return (x - F(x) / F1(x));
}

double root(func_t f, func_t g, func_t f1, func_t g1, double a, double b, double eps1) {
    double F(double x) {
        return f(x) - g(x);
    }

    double F1(double x) {
        return f1(x) - g1(x);
    }

    double eps_cur = b - a;
    double x1, x2;
    if ((F(b) - F(a)) * ((b - a) / 2 - F((b - a) / 2)) > 0){
        x1 = a, x2 = b;
    } else {
        x1 = b, x2 = a;
    }
    while (eps_cur > eps1) {
        x1 = chordIteration(F, a, b, x1);
        x2 = tangentIteration(F, F1, a, b, x2);
        eps_cur = x1 - x2;
        eps_cur = eps_cur < 0 ? -eps_cur : eps_cur;
    }
    return x1;
}

double f1(double x) {
    return 3 * (0.5 / (x + 1) + 1);
}

double f1_dif(double x) {
    return -1.5 / ((x + 1) * (x + 1));
}

double f2(double x) {
    return 2.5 * x - 9.5;
}

double f2_dif(double x) {
    return 2.5;
}

double f3(double x) {
    return 5 / x;
}

double f3_dif(double x) {
    return -5 / (x * x);
}

int main(void)
{
    //f1 = 3 (0.5 / (x + 1) + 1), f2 = 2.5x − 9.5, f3 = 5 / x, x > 0
    //f1 intersect f2 in [4;6]
    printf("%f\n", root(f1, f2, f1_dif, f2_dif, 4, 6, 0.001));
    //f1 intersect f3 in [1;2]
    printf("%f\n", root(f1, f3, f1_dif, f3_dif, 1, 2, 0.001));
    //f2 intersect f3 in [3;5]
    printf("%f\n", root(f2, f3, f2_dif, f3_dif, 3, 5, 0.001));
    return 0;
}
