/*
 * main.c
 *
 * Пример использования гибридной сортировки Min-Max для int и double.
 */

#include "min_max_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand((unsigned)time(NULL));

    /* --- Тест для int --- */
    const int n_int = 20;
    int arr_int[n_int];
    printf("Исходный массив int:\n");
    for (int i = 0; i < n_int; i++) {
        arr_int[i] = rand() % 100;
        printf("%d ", arr_int[i]);
    }
    printf("\n");

    hybrid_min_max_sort_serial(arr_int, 0, n_int - 1, 2);

    printf("Отсортированный массив int:\n");
    for (int i = 0; i < n_int; i++) {
        printf("%d ", arr_int[i]);
    }
    printf("\n\n");

    /* --- Тест для double --- */
    const int n_double = 20;
    double arr_double[n_double];
    printf("Исходный массив double:\n");
    for (int i = 0; i < n_double; i++) {
        arr_double[i] = ((double)rand() / RAND_MAX) * 100.0;
        printf("%.2lf ", arr_double[i]);
    }
    printf("\n");

    hybrid_min_max_sort_serial_double(arr_double, 0, n_double - 1, 2);

    printf("Отсортированный массив double:\n");
    for (int i = 0; i < n_double; i++) {
        printf("%.2lf ", arr_double[i]);
    }
    printf("\n");

    return 0;
}

