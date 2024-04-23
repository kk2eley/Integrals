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

double integral(func_t f, double a, double b, double eps2) {
    int n = 1000;
    double eps_cur = 0;
    double shift = 0;
    double step = (double)(b - a) / n;
    double sum = 0;
    double res = 0;
    double prev_res = 0;

    //base res
    for (int i = 1; i < n; i ++) {
        sum += f(a + step * i);
    }
    res = ((b - a) / 2) * (f(a) + f(b) + 2 * sum) / n;
    n *= 2;
    shift = step / 2;
    do {
        prev_res = res;
        // |       !       !       !       |  n = 4 k = 4 i = 1, 2, 3
        // |   !   |   !   |   !   |   !   |  n = 8 k = 4 i = 1, 2, 3, 4
        // | ! | ! | ! | ! | ! | ! | ! | ! |  n = 16 k = 8 i = 1, 2, 3
        for (int i = 0; i < n/2; i++) {
            sum += f(a + shift + step * i);
        }
        res = ((b - a) / 2) * (f(a) + f(b) + 2 * sum) / n;
        eps_cur = res - prev_res;
        eps_cur = eps_cur < 0 ? -eps_cur : eps_cur;
        n *= 2;
        step = (double)(b - a) / n;
    } while (eps_cur > eps2);
    return prev_res;
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

double test_interal_f1(double x) {//y = x
    return x;
}

double test_interal_f2(double x) {//y = x^2
    return x * x;
}

double test_interal_f3(double x) {//y = 1/x
    return 1 / x;
}

double test_root_f1(double x) {//y = x
    return x;
}

double test_root_f1_dif(double x) {//y = (x)'
    return 1;
}

double test_root_f2(double x) {//y = -x + 1
    return -x + 2;
}

double test_root_f2_dif(double x) {//y = (-x + 1)'
    return -1;
}

double test_root_f3(double x) {//y = (x-1)^2 - 1
    return (x - 1) * (x - 1) - 1;
}

double test_root_f3_dif(double x) {//y = ((x-1)^2 - 1)'
    return 2 * (x-1);
}

int main(void)
{
    printf("Test functions for root\n");
    printf("y = x intersect y = -x + 1 on [0; 2]: %f\n", root(test_root_f1, test_root_f2, test_root_f1_dif, test_root_f2_dif, 0, 2, 0.001));
    printf("y = x intersect y = (x-1)^2 - 1 on [-0.5; 0.5]: %f\n", root(test_root_f1, test_root_f3, test_root_f1_dif, test_root_f3_dif, -0.5, 0.5, 0.001));
    printf("y = -x + 1 intersect y = (x-1)^2 - 1 on [1; 3]: %f\n", root(test_root_f2, test_root_f3, test_root_f2_dif, test_root_f3_dif, 1, 3, 0.001));
    printf("\n");
    printf("Test functions for integral\n");
    printf("integral y = x on [0; 2]: %f =?= 2\n", integral(test_interal_f1, 0, 2, 0.001));
    printf("integral y = x^2 on [0; 3]: %f =?= 9\n", integral(test_interal_f2, 0, 3, 0.001));
    printf("integral y = 1/x on [1; 5]: %f =?= 1.609\n", integral(test_interal_f3, 1, 5, 0.001));
    printf("\n");
    printf("Real task\n");
    printf("f1 = 3 (0.5 / (x + 1) + 1), f2 = 2.5x − 9.5, f3 = 5 / x, x > 0\n");

    printf("f1 intersect f2 on [4;6]: %f\n", root(f1, f2, f1_dif, f2_dif, 4, 6, 0.001));
    printf("f1 intersect f3 on [1;2]: %f\n", root(f1, f3, f1_dif, f3_dif, 1, 2, 0.001));
    printf("f2 intersect f3 on [3;5]: %f\n", root(f2, f3, f2_dif, f3_dif, 3, 5, 0.001));
    printf("Square of triangle: %f", integral(f1, root(f1, f3, f1_dif, f3_dif, 1, 2, 0.001), root(f1, f2, f1_dif, f2_dif, 4, 6, 0.001), 0.001) -
    integral(f2, root(f2, f3, f2_dif, f3_dif, 3, 5, 0.001), root(f1, f2, f1_dif, f2_dif, 4, 6, 0.001), 0.001) -
    integral(f3, root(f1, f3, f1_dif, f3_dif, 1, 2, 0.001), root(f2, f3, f2_dif, f3_dif, 3, 5, 0.001), 0.001));
    return 0;
}
