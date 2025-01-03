#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h> 

#define precision 0.015
#define FILENAME "output.csv"
#define MAX_T 100

int count = 0;
double* values;
double F1(double);
double F2(double);
double F3(double);

int input_file(char*);
int output_to_file(double values[], int);
double find_max(double values[], int);
double find_min(double values[], int);
unsigned long long factorial(int n);
int linear_search(double values[], int, double);
double differentiate(double (*func)(double), double, double);

int main() {
    setlocale(LC_CTYPE, "RUS");
    int mode, n;
    double xmin, xmax, dx;
    double (*func)(double x);
    func = F1;

    while (1) {
        printf("\nМеню:\n");
        printf("0) Выбор функции для работы(по умолчанию F1)\n");
        printf("1) Вывод таблицы значений на экран и в файл\n");
        printf("2) Определение минимального и максимального из вычисленных значений\n");
        printf("3) Линейный поиск значения аргумента при заданном значении функции\n");
        printf("4) Дифференцирование функции в заданной точке\n");
        printf("5) Задание режима и параметров расчета\n");
        printf("6) Завершение работы\n");
        printf("Выберите опцию: ");
        scanf("%d", &mode);
        switch (mode) {
        case 0: {
            int choice_func;
            printf("Выберите функцию(F1 - 1,F2 - 2,F3 - 3):\n");
            scanf("%d", &choice_func);
            double x = 0;
            switch (choice_func) {
            case 1: { func = F1; break; }
            case 2: { func = F2; break; }
            case 3: { func = F3; break; }
            default: func = F1;
            }

        }

        case 1:
            if (count == 0) {
                printf("Нет вычисленных значений.\n");
            }
            else {
                for (int i = 0; i < count; i++) {
                    printf("|%3.d | %11.4lf |\n", i + 1, values[i]);
                }
                output_to_file(values, count);
            }
            break;
        case 2:
            if (count == 0) {
                printf("Нет вычисленных значений.\n");
            }
            else {
                printf("Минимальное значение: %lf\n", find_min(values, count));
                printf("Максимальное значение: %lf\n", find_max(values, count));
            }
            break;
        case 3: {
            double target;
            printf("Введите значение функции для поиска: ");
            scanf("%lf", &target);
            int index = linear_search(values, count, target);
            if (index != -1) {
                printf("Значение найдено на индексе: %d\n", index);
            }
            else {
                printf("Значение не найдено.\n");
            }
            break;
        }
        case 4: {
            double x, h;
            printf("Введите точку для дифференцирования: ");
            scanf("%lf", &x);
            printf("Введите приращение h: ");
            scanf("%lf", &h);
            double derivative = differentiate(func, x, h);
            printf("Производная в точке %lf: %lf\n", x, derivative);
            break;
        }
        case 5: {
            int choice;
            printf("Выберите режим:\n");
            printf("1) Заданные значения (cvs-файл)\n");
            printf("2) Сгенерированные от xmin с шагом dx(n=10)\n");
            printf("3) Случайные значения из интервала (xmin; xmax)\n");
            printf("Выберите режим: ");
            scanf("%d", &choice);

            switch (choice) {
            case 1: {
                printf("Введите имя файла\n");
                char inputfile[255];
                scanf("%s", &inputfile);
                input_file(inputfile);
                for (int i = 0; i < count; i++)
                    values[i] = func(values[i]);
                break;
            }
            case 2: {
                printf("Введите xmin: ");
                scanf("%lf", &xmin);
                printf("Введите шаг dx: ");
                scanf("%lf", &dx);
                n = 10;
                count = 0;
                values = (double*)malloc(n * sizeof(double));
                for (double x = xmin; count < n; x += dx) {
                    values[count++] = func(x);
                }
                break;
            }
            case 3: {

                printf("Сколько значений? ");
                scanf("%d", &n);
                printf("Введите xmin: ");
                scanf("%lf", &xmin);
                printf("Введите xmax: ");
                scanf("%lf", &xmax);
                values = (double*)malloc(n * sizeof(double));
                count = 0;
                while (count < n) {
                    {
                        double x = xmin + 1.f * (xmax - xmin) * rand() / RAND_MAX;
                        values[count++] = func(x);
                    };
                }
                break;
            }
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
            }
            break;
        }
        case 6:
            printf("Завершение работы.\n");
            return 0;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }

    return 0;
}

double differentiate(double (*func)(double), double x, double h) {
    return (func(x + h) - func(x)) / h;
}

int linear_search(double values[], int count, double target) {
    for (int i = 0; i < count; i++) {
        if (fabs(values[i] - target) < precision) {
            return i;
        }
    }
    return -1;
}

int input_file(char* inputfile) {
    FILE* file = fopen(inputfile, "r");
    if (file == NULL) {
        return -1;
    }
    values = (double*)malloc(sizeof(file));
    count = 0;
    while (fscanf(file, "%lf", &values[count]) != EOF) {
        count++;
    }
    fclose(file);
}

int output_to_file(double values[], int count) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        return -1;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d, %lf\n", i + 1, values[i]);
    }
    fclose(file);
    printf("Данные записаны в файл %s\n", FILENAME);
}

double find_max(double values[], int count) {
    double max;
    max = values[0];
    for (int i = 1; i < count; i++) {
        if (values[i] > max) max = values[i];
    }
    return max;
}
double find_min(double values[], int count) {
    double min;
    min = values[0];
    for (int i = 1; i < count; i++) {
        if (values[i] < min) min = values[i];
    }
    return min;
}

unsigned long long factorial(int n) {
    if (n == 0 || n == 1) return 1;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double F1(double x) {
    return log(fabs(sin(x)) + pow(x, 2.0 / 5.0)) / log(5);
}
double F2(double x) {
    if (x <= 1) {
        return (pow(x, 2) - 7 * x + 3) / exp(x);
    }
    else {
        return log10((pow(x, 2)));
    }

}
double F3(double x) {
    if (x > 1)
    {
        double sum = 0.0;
        for (int n = 1; n < MAX_T; n++) {
            double term = ((double)factorial(2 * n) / ((pow(2, 2 * n) * pow(factorial(n), 2)))) * (pow(x, -2 * n) / (2 * n));
            sum += term;
            if (fabs(term) < precision) {
                break;
            }
        }
        return log(2 * x) - sum;
    }
    else return 0;
}
