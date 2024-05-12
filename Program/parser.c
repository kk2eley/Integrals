#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct Node {
    char value[32]; // Значение токена
    char const_name[32];  // Имя константы, если узел содержит число
    struct Node* left;
    struct Node* right;
} Node;

// Прототипы функций
Node* parse_rpn(char* expression);
void generate_asm(Node* root, FILE* out);
void free_tree(Node* node);
void print_tree(Node* root, int level);

// Глобальный список и счетчик для генерации уникальных имен констант
char constants[100][64];  // Допустим, мы ограничимся 100 константами
int const_count = 0;

double leftborder, rightborder;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <spec_file>\n", argv[0]);
        return 1;
    }

    FILE* in = fopen(argv[1], "r");
    if (!in) {
        perror("Error opening file");
        return 1;
    }

    double a, b;
    char lines[3][256];  // Строки для хранения функций
    Node* trees[3];      // Массив указателей на деревья

    if (fscanf(in, "%lf %lf\n", &a, &b) != 2) {
        fprintf(stderr, "Failed to read interval bounds\n");
        fclose(in);
        return 1;
    }

    // Чтение функций
    for (int i = 0; i < 3; i++) {
        if (!fgets(lines[i], sizeof(lines[i]), in)) {
            fprintf(stderr, "Failed to read function %d\n", i + 1);
            fclose(in);
            return 1;
        }
        if (lines[i][strlen(lines[i]) - 1] == '\n') lines[i][strlen(lines[i]) - 1] = 0; // Удаление символа новой строки
        printf("Tree number %d:\n", i);
        trees[i] = parse_rpn(lines[i]); // Парсинг функций и построение деревьев
        print_tree(trees[i], 0);
    }

    fclose(in);

    FILE* out = fopen("rpnfunctions.asm", "w"); // Создание файла для ассемблерного кода
    fprintf(out, "section .data\n");
    fprintf(out, "leftborder dq %.10f\n", a);
    fprintf(out, "rightborder dq %.10f\n", b);
    for (int i = 0; i < const_count; i++) {
        fprintf(out, "%s\n", constants[i]); // Создаём константы с числами для функций
    }

    // Генерация ассемблерного кода
    fprintf(out, "section .text\n");
    fprintf(out, "global fh1\nglobal fh2\nglobal fh3\nglobal leftborder\nglobal rightborder\n");
    for (int i = 0; i < 3; i++) {
        fprintf(out, "fh%d:\n", i + 1);
        fprintf(out, "  push ebp\n");
        fprintf(out, "  mov ebp, esp\n");
        fprintf(out, "  finit\n");
        generate_asm(trees[i], out);
        fprintf(out, "  leave\n");
        fprintf(out, "  ret\n");
        free_tree(trees[i]);
    }

    fclose(out);
    return 0;
}

// Функции для анализа RPN, построения дерева и генерации ассемблера
Node* parse_rpn(char* expression) {
    //2 x 4 / tan –
    char* token = strtok(expression, " "); // Разбиваем строку по пробелам
    Node* stack[64];  // Допустим, стек у нас достаточно велик
    int stack_size = 0; // Индекс вершины стека

    while (token) { // Пока указатель не NULL
        Node* node = malloc(sizeof(Node)); // Создаём новую вершину
        strcpy(node->value, token);
        node->left = node->right = NULL;

        // Проверяем, является ли токен числом
        if (isdigit(token[0]) || token[0] == '.') {
            // Генерируем имя для константы и сохраняем ее значение
            sprintf(node->const_name, "const%d", const_count++);
            char* endptr;
            strtol(token, &endptr, 10); // Попытка преобразовать строку в целое число
            if (*endptr == '\0') { // Если вся строка успешно преобразовалась, значит это целое число
                sprintf(constants[const_count-1], "%s dq %s.0", node->const_name, token); // Добавляем .0 для целого числа
            } else {
                sprintf(constants[const_count-1], "%s dq %s", node->const_name, token); // Оставляем как есть, если уже вещественное
            }
        }
        // Определение, является ли токен оператором
        if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/' ||
            strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 ||
            strcmp(token, "tan") == 0 || strcmp(token, "ctg") == 0) {
            node->right = stack[--stack_size];  // Правый операнд
            if (token[0] != 's' && token[0] != 'c' && token[0] != 't') { // Если не унарный оператор
                node->left = stack[--stack_size];  // Левый операнд
            }
        }
        stack[stack_size++] = node; // Кладём вершину обратно в стэк
        token = strtok(NULL, " "); // Берём следующий токен
    }

    return stack_size > 0 ? stack[stack_size - 1] : NULL; // Возвращаем указатель на корень дерева
}

void generate_asm(Node* root, FILE* out) {
    if (!root) return; // Если корня нет

    generate_asm(root->left, out);
    generate_asm(root->right, out);

    fprintf(out, "  ; %s\n", root->value);
    if (root->const_name[0]) {  // Если у узла есть имя константы
        fprintf(out, "  fld qword [%s]\n", root->const_name); // Используем имя константы
    }else if (strcmp(root->value, "+") == 0) {
        fprintf(out, "  faddp\n");
    } else if (strcmp(root->value, "-") == 0) {
        fprintf(out, "  fsubp\n");
    } else if (strcmp(root->value, "*") == 0) {
        fprintf(out, "  fmulp\n");
    } else if (strcmp(root->value, "/") == 0) {
        fprintf(out, "  fdivp\n");
    } else if (strcmp(root->value, "sin") == 0) {
        fprintf(out, "  fsin\n");
    } else if (strcmp(root->value, "cos") == 0) {
        fprintf(out, "  fcos\n");
    } else if (strcmp(root->value, "tan") == 0) {
        fprintf(out, "  fptan\n  fstp st0\n");
    } else if (strcmp(root->value, "ctg") == 0) {
        fprintf(out, "  fptan\n  fdivp\n");
    } else if (strcmp(root->value, "e") == 0) {
        fprintf(out, "  fld1\n"); // Загружаем значение 1 на стек FPU
        fprintf(out, "    fldl2e\n"); // Загружаем log2(e) на стек FPU
        fprintf(out, "    fscale\n"); // Вычисляем 2^(ST(1)) * ST(0) и помещаем результат в ST(0)
        fprintf(out, "    fstp st1\n"); //Удаляем log2(e) из стека, оставляя e в ST(0)
    } else if (strcmp(root->value, "pi") == 0) {
        fprintf(out, "  fldpi\n");
    } else if (strcmp(root->value, "x") == 0){
        fprintf(out, "  fld qword[ebp + 8]\n");
    }
}

void print_tree(Node* root, int level) {
    if (root == NULL) {
        return; // Если узел пуст, ничего не делаем
    }

    // Печатаем правое поддерево с увеличенным отступом
    print_tree(root->right, level + 1);

    // Печатаем текущий узел
    for (int i = 0; i < level; i++) {
        printf("    "); // Добавляем отступы в зависимости от уровня
    }
    printf("%s\n", root->value); // Печатаем значение узла

    // Печатаем левое поддерево с увеличенным отступом
    print_tree(root->left, level + 1);
}

void free_tree(Node* node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}
