#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef double (*func_t)(double);

double eps1 = 0.0001, eps2 = 0.0001;

int num_of_iterations = 0;

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


#if defined(CHORD)
    double root(func_t f, func_t f1, func_t g, func_t g1, double a, double b, double eps1) {
        double F(double x) {
            return f(x) - g(x);
        }

        double eps_cur = b - a;
        double x, xprev;
        if ((F(b) - F(a)) * ((b - a) / 2 - F((b - a) / 2)) > 0){
            x = a;
        } else {
            x = b;
        }
        num_of_iterations = 0;
        while (eps_cur > eps1) {
            xprev = x;
            x = chordIteration(F, a, b, x);
            eps_cur = x - xprev;
            eps_cur = eps_cur < 0 ? -eps_cur : eps_cur;
            num_of_iterations++;
        }
        return x;
    }
#elif defined(TANGENT)
    double root(func_t f, func_t f1, func_t g, func_t g1, double a, double b, double eps1) {
        double F(double x) {
            return f(x) - g(x);
        }

        double F1(double x) {
            return f1(x) - g1(x);
        }

        double eps_cur = b - a;
        double x, xprev;
        if ((F(b) - F(a)) * ((b - a) / 2 - F((b - a) / 2)) > 0){
            x = b;
        } else {
            x = a;
        }
        num_of_iterations = 0;
        while (eps_cur > eps1) {
            xprev = x;
            x = tangentIteration(F, F1, a, b, x);
            eps_cur = x - xprev;
            eps_cur = eps_cur < 0 ? -eps_cur : eps_cur;
            num_of_iterations++;
        }
        return x;
    }
#else
    double root(func_t f, func_t f1, func_t g, func_t g1, double a, double b, double eps1) {
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
        num_of_iterations = 0;
        while (eps_cur > eps1) {
            x1 = chordIteration(F, a, b, x1);
            x2 = tangentIteration(F, F1, a, b, x2);
            eps_cur = x1 - x2;
            eps_cur = eps_cur < 0 ? -eps_cur : eps_cur;
            num_of_iterations++;
        }
        return x1;
    }
#endif



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

/*double f1(double x) {
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
}*/


double f1 (double x);

double f2 (double x);

double f3 (double x);

double df1(double);

double df2(double x);

double df3(double x);

double f4(double x) {//y = x
    return x;
}

double df4(double x) {//y = (x)'
    return 1;
}

double f5(double x) {//y = -x + 1
    return -x + 2;
}

double df5(double x) {//y = (-x + 1)'
    return -1;
}

double f6(double x) {//y = (x-1)^2 - 1
    return (x - 1) * (x - 1) - 1;
}

double df6(double x) {//y = ((x-1)^2 - 1)'
    return 2 * (x-1);
}

//массивы функций (чтобы функцию по номеру вызывать)
double (*functions[6])(double) = {&f1, &f2, &f3, &f4, &f5, &f6};
double (*derivs[6])(double) = {&df1, &df2, &df3, &df4, &df5, &df6};


void print_del(void) {
    printf("======================================\n");
}

void root_iterations(int num_a, int num_b, double a, double b){
    num_of_iterations = 0;
    root(functions[num_a], derivs[num_a], functions[num_b], derivs[num_b], a, b, eps1);
    print_del();
    printf("number of iterations: %d\n", num_of_iterations);
    print_del();
}

void print_help(void) {
    print_del();
    printf("Task functions:\n");
    printf("f1: y = 3 * (0.5 / (x + 1) + 1)\n");
    printf("f2: y = 2.5 * x - 9.5\n");
    printf("f3: y = 5 / x\n");
    printf("f4: y = x (for tests)\n");
    printf("f5: y = -x + 1 (for tests)\n");
    printf("f6: y = (x-1)^2 - 1 (for tests)\n");
    print_del();
    printf("To show intersection points use flag: -roots\n");
    printf("To show value of integral on segment between roots use flag: -integrals\n");
    #if defined(HANDLE)
    printf("To show intersection points of functions that you entered by hands use -hdl-roots\n");
    printf("TO show answer for your own functions use: -hdl-answer\n");
    #endif // defined
    printf("To show help information use flag: -help\n");
    printf("To show number of iterations you need to calculate intersection use: -iters <number of first func> <number of second func> <left border> <right border>\n");
    printf("borders for functions from task:\n");
    printf("f1 intersects f2 on [4;6]\n");
    printf("f1 intersects f3 on [1;2]\n");
    printf("f2 intersects f3 on [3;5]\n");
    printf("To find intersection of functions use: -test-roots <number of first func> <number of second func> <left border> <right border>\n");
    printf("To calculate integral on segment use: -test-integrals <number of function> <left border> <right border>\n");
    print_del();
}

void print_roots(void) {
    print_del();
    printf("fh1 intersect fh2: %f\n", root(f1, df1, f2, df2, 4, 6, eps1));
    printf("f1 intersect f3 on [1;2]: %f\n", root(f1, df1, f3, df3, 1, 2, eps1));
    printf("f2 intersect f3 on [3;5]: %f\n", root(f2, df2, f3, df3, 3, 5, eps1));
    print_del();
}

void print_integrals(void) {
    double root1 = root(f1, df1, f2, df2, 4, 6, eps1);
    double root2 = root(f1, df1, f3, df3, 1, 2, eps1);
    double root3 = root(f3, df3, f2, df2, 3, 5, eps1);
    double I_f1 = integral(f1, root1, root2, eps2);
    double I_f2 = integral(f2, root3, root2, eps2);
    double I_f3 = integral(f3 ,root1, root3, eps2);
    print_del();
    printf("integral f1: %lf\nintegral f2: %lf\nintegral f3: %lf\n", I_f1, I_f2, I_f3);
    print_del();
}

void print_iters(int num_a, int num_b, double a, double b){
    root(functions[num_a], derivs[num_a], functions[num_b], derivs[num_b], a, b, eps1);
    print_del();
    printf("Number of iterations: %d\n", num_of_iterations);
    print_del();
}

//поиск пересечения между двумя функциями(для отладки)
void test_roots(int num_a, int num_b, double a, double b){
    double cur_root = root(functions[num_a], derivs[num_a], functions[num_b], derivs[num_b], a, b, eps1);
    print_del();
    printf("Intersection of f%d and f%d on [%lf, %lf] segment: %lf \n", num_a + 1, num_b + 1, a, b, cur_root);
    print_del();
}
//поиск интеграла на данном промежутке(для отладки)
void test_integrals(int num, double a, double b){
    double cur_integral = integral(functions[num], a, b, eps2);
    print_del();
    printf("Value of integral of f%d on [%lf, %lf] segment: %lf \n", num + 1, a, b, cur_integral);
    print_del();
}

/*
    print_del();
    printf("Test functions for root\n");
    printf("y = x intersect y = -x + 1 on [0; 2]: %f\n", root(test_root_f1, test_root_f2, test_root_f1_dif, test_root_f2_dif, 0, 2, 0.001));
    printf("y = x intersect y = (x-1)^2 - 1 on [-0.5; 0.5]: %f\n", root(test_root_f1, test_root_f3, test_root_f1_dif, test_root_f3_dif, -0.5, 0.5, 0.001));
    printf("y = -x + 1 intersect y = (x-1)^2 - 1 on [1; 3]: %f\n", root(test_root_f2, test_root_f3, test_root_f2_dif, test_root_f3_dif, 1, 3, 0.001));
    printf("\n");
    printf("Test functions for integral\n");
    printf("integral y = x on [0; 2]: %f =?= 2\n", integral(test_interal_f1, 0, 2, 0.001));
    printf("integral y = x^2 on [0; 3]: %f =?= 9\n", integral(test_interal_f2, 0, 3, 0.001));
    printf("integral y = 1/x on [1; 5]: %f =?= 1.609\n", integral(test_interal_f3, 1, 5, 0.001));
    print_del();
*/

void print_answer(void) {
    print_del();
    printf("Real task\n");
    printf("f1 = 3 (0.5 / (x + 1) + 1), f2 = 2.5x − 9.5, f3 = 5 / x, x > 0\n");
    printf("Square of triangle: %f\n", integral(f1, root(f1, df1, f3, df3, 1, 2, eps1), root(f1, df1, f2, df2, 4, 6, eps1), eps2) -
    integral(f2, root(f2, df2, f3, df3, 3, 5, eps1), root(f1, df1, f2, df2, 4, 6, eps1), eps2) -
    integral(f3, root(f1, df1, f3, df3, 1, 2, eps1), root(f2, df2, f3, df3, 3, 5, eps1), eps2));
}

//выдать ошибку, если аргументы командной строки некорректны
void print_exeption(void){
    print_del();
    printf("you have used incorrect arguments of command line.\n Please use -help to check list of supported commands\n ");
    print_del();
}

#if defined(HANDLE)
double fh1(double x);
double fh2(double x);
double fh3(double x);

extern double leftborder, rightborder;

void print_hdl_roots(void) {
    print_del();
    printf("fh1 intersect fh2 on [%f;%f]: %f\n",leftborder, rightborder, root(fh1, NULL, fh2, NULL, leftborder, rightborder, eps1));
    printf("fh1 intersect fh3 on [%f;%f]: %f\n",leftborder, rightborder, root(fh1, NULL, fh3, df3, leftborder, rightborder, eps1));
    printf("fh2 intersect fh3 on [%f;%f]: %f\n",leftborder, rightborder, root(fh2, NULL, fh3, NULL, leftborder, rightborder, eps1));
    print_del();
}

void print_hdl_answer(void) {
    print_del();
    double I1 = integral(fh1, root(fh1, NULL, fh2, NULL, leftborder, rightborder, eps1), root(fh1, NULL, fh3, df3, leftborder, rightborder, eps1), eps2);
    double I2 = integral(f2, root(fh2, NULL, fh3, NULL, leftborder, rightborder, eps1), root(fh1, NULL, fh2, NULL, leftborder, rightborder, eps1), eps2);
    double I3 = integral(fh3, root(fh1, NULL, fh3, df3, leftborder, rightborder, eps1), root(fh2, NULL, fh3, NULL, leftborder, rightborder, eps1), eps2);
    double I = I1 + I2 + I3;
    I = I < 0 ? -I : I;
    printf("Square of handle entered functions: %f\n", I);
}
#endif

int main (int argc, char ** argv) {

    #if defined(HANDLE)
    int num_of_commands = 9;
    char * commands[] = {"-roots", "-integrals", "-answer", "-help", "-iters", "-test-roots" ,"-test-integrals", "-hdl-roots", "-hdl-answer"};//список команд
    #else
    int num_of_commands = 7;
    char * commands[] = {"-roots", "-integrals", "-answer", "-help", "-iters", "-test-roots" ,"-test-integrals"};//список команд
    #endif // defined
    int  cur_arg = 1;

    while (cur_arg < argc){//обработка аргументов командной строки
        int cur_num = 0;
        for (int i = 0; i < num_of_commands; i++){//соотнести аргумент с его номером для работы switch
            if (strcmp(argv[cur_arg], commands[i]) == 0){
                cur_num = i + 1;
            }
        }
        int f_a, f_b;
        double a, b;
        switch (cur_num)
        {
        case 1: //напечатать корни для функций из задания
            print_roots();
            break;

        case 2: // напечатать интегралы функций из задания
            print_integrals();
            break;

        case 3://напечатать ответ на задачу
            print_answer();
            break;

        case 4: // напечатать список команд
            print_help();
            break;
        case 5: // вычислить число итераций root
            f_a = atoi(argv[cur_arg + 1]), f_b = atoi(argv[cur_arg + 2]);
            a = atof(argv[cur_arg + 3]), b = atof(argv[cur_arg + 4]);
            f_a--;
            f_b--;
            cur_arg += 4;
            print_iters(f_a, f_b, a, b);
            break;
        case 6:// вычислить точку пересечения двух выбранных функций
            f_a = atoi(argv[cur_arg + 1]), f_b = atoi(argv[cur_arg + 2]);
            a = atof(argv[cur_arg + 3]), b = atof(argv[cur_arg + 4]);
            f_a--;
            f_b--;
            cur_arg += 4;
            test_roots(f_a, f_b, a, b);
            break;
        case 7:// вычислить интеграл от выбранной функции на введенном промежутке
            f_a = atoi(argv[cur_arg + 1]);
            a = atof(argv[cur_arg + 2]), b = atof(argv[cur_arg + 3]);
            f_a--;
            cur_arg += 3;
            test_integrals(f_a, a, b);
            break;
        #if defined(HANDLE)
        case 8:
            print_hdl_roots();
            break;
        case 9:
            print_hdl_answer();
            break;
        #endif // defined
        default:
        print_exeption(); // ввели неправильную команду
            break;
        }
        cur_arg++;
    }
    return 0;
}
