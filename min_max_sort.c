/*
 * min_max_sort.c
 *
 * Реализация гибридной сортировки Min-Max для типов int и double.
 */

#include "min_max_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THRESHOLD_DEFAULT 64

/* ==================== Вспомогательные функции ==================== */

// Обмен значений для int
static inline void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Обмен значений для double
static inline void swap_double(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

/* ==================== Функции сортировки для int ==================== */

// Сортировка вставками для int
void insertion_sort(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Адаптивный порог (если размер сегмента меньше THRESHOLD_DEFAULT, то используем его, иначе – THRESHOLD_DEFAULT)
int get_adaptive_threshold(int segment_size) {
    return (segment_size < THRESHOLD_DEFAULT) ? segment_size : THRESHOLD_DEFAULT;
}

// Медиана из 5 элементов (возвращает индекс)
int median_of_five_index(const int arr[], int i1, int i2, int i3, int i4, int i5) {
    int indices[5] = { i1, i2, i3, i4, i5 };
    for (int i = 1; i < 5; i++) {
        int temp = indices[i];
        int j = i - 1;
        while (j >= 0 && arr[indices[j]] > arr[temp]) {
            indices[j + 1] = indices[j];
            j--;
        }
        indices[j + 1] = temp;
    }
    return indices[2];
}

// Медиана из 3 элементов (возвращает индекс)
int median_of_three_index(const int arr[], int i1, int i2, int i3) {
    if (arr[i1] < arr[i2]) {
        if (arr[i2] < arr[i3])
            return i2;
        else if (arr[i1] < arr[i3])
            return i3;
        else
            return i1;
    } else {
        if (arr[i1] < arr[i3])
            return i1;
        else if (arr[i2] < arr[i3])
            return i3;
        else
            return i2;
    }
}

// Выбор нижнего опорного элемента (для левого сегмента)
int select_lower_pivot(const int arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        return median_of_three_index(arr, left, left + segment_size / 4, left + segment_size / 2);
    } else {
        return median_of_five_index(arr, left, left + segment_size / 8, left + segment_size / 4,
                                     left + (3 * segment_size) / 8, left + segment_size / 2);
    }
}

// Выбор верхнего опорного элемента (для правого сегмента)
int select_upper_pivot(const int arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        return median_of_three_index(arr, left + segment_size / 2, left + (3 * segment_size) / 4, right);
    } else {
        return median_of_five_index(arr, left + segment_size / 2, left + (5 * segment_size) / 8,
                                     left + (3 * segment_size) / 4, left + (7 * segment_size) / 8, right);
    }
}

// Классическое слияние двух отсортированных частей массива
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    if (!L || !R) {
        fprintf(stderr, "Ошибка выделения памяти в merge.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(L, &arr[left], n1 * sizeof(int));
    memcpy(R, &arr[mid + 1], n2 * sizeof(int));
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
    free(L);
    free(R);
}

// Классическая сортировка слиянием
void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Гибридная сортировка для int
void hybrid_min_max_sort_serial(int arr[], int left, int right, int k) {
    int segment_size = right - left + 1;
    int threshold = get_adaptive_threshold(segment_size);
    if (segment_size <= threshold) {
        insertion_sort(arr, left, right);
        return;
    }
    
    int i_med_low = select_lower_pivot(arr, left, right, segment_size);
    int lowerPivot = arr[i_med_low];
    int i_med_high = select_upper_pivot(arr, left, right, segment_size);
    int upperPivot = arr[i_med_high];
    
    if (lowerPivot > upperPivot) {
        swap(&arr[i_med_low], &arr[i_med_high]);
        lowerPivot = arr[i_med_low];
        upperPivot = arr[i_med_high];
    }
    
    int l = left, r = right;
    for (int i = left; i <= r;) {
        if (arr[i] < lowerPivot) {
            swap(&arr[i], &arr[l]);
            l++;
            i++;
        } else if (arr[i] > upperPivot) {
            swap(&arr[i], &arr[r]);
            r--;
        } else {
            i++;
        }
    }
    
    if (l == left || r == right) {
        merge_sort(arr, left, right);
        return;
    }
    
    hybrid_min_max_sort_serial(arr, left, l - 1, k);
    hybrid_min_max_sort_serial(arr, l, r, k);
    hybrid_min_max_sort_serial(arr, r + 1, right, k);
}

/* ==================== Функции сортировки для double ==================== */

// Сортировка вставками для double
void insertion_sort_double(double arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        double key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Медиана из 5 элементов для double (возвращает индекс)
int median_of_five_index_double(const double arr[], int i1, int i2, int i3, int i4, int i5) {
    int indices[5] = { i1, i2, i3, i4, i5 };
    for (int i = 1; i < 5; i++) {
        int temp = indices[i];
        int j = i - 1;
        while (j >= 0 && arr[indices[j]] > arr[temp]) {
            indices[j + 1] = indices[j];
            j--;
        }
        indices[j + 1] = temp;
    }
    return indices[2];
}

// Медиана из 3 элементов для double (возвращает индекс)
int median_of_three_index_double(const double arr[], int i1, int i2, int i3) {
    if (arr[i1] < arr[i2]) {
        if (arr[i2] < arr[i3])
            return i2;
        else if (arr[i1] < arr[i3])
            return i3;
        else
            return i1;
    } else {
        if (arr[i1] < arr[i3])
            return i1;
        else if (arr[i2] < arr[i3])
            return i3;
        else
            return i2;
    }
}

// Выбор нижнего опорного элемента для double
int select_lower_pivot_double(const double arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        return median_of_three_index_double(arr, left, left + segment_size / 4, left + segment_size / 2);
    } else {
        return median_of_five_index_double(arr, left, left + segment_size / 8, left + segment_size / 4,
                                            left + (3 * segment_size) / 8, left + segment_size / 2);
    }
}

// Выбор верхнего опорного элемента для double
int select_upper_pivot_double(const double arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        return median_of_three_index_double(arr, left + segment_size / 2, left + (3 * segment_size) / 4, right);
    } else {
        return median_of_five_index_double(arr, left + segment_size / 2, left + (5 * segment_size) / 8,
                                            left + (3 * segment_size) / 4, left + (7 * segment_size) / 8, right);
    }
}

// Слияние для double
void merge_double(double arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    double *L = malloc(n1 * sizeof(double));
    double *R = malloc(n2 * sizeof(double));
    if (!L || !R) {
        fprintf(stderr, "Ошибка выделения памяти в merge_double.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(L, &arr[left], n1 * sizeof(double));
    memcpy(R, &arr[mid + 1], n2 * sizeof(double));
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
    free(L);
    free(R);
}

// Классическая сортировка слиянием для double
void merge_sort_double(double arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_double(arr, left, mid);
        merge_sort_double(arr, mid + 1, right);
        merge_double(arr, left, mid, right);
    }
}

// Гибридная сортировка для double
void hybrid_min_max_sort_serial_double(double arr[], int left, int right, int k) {
    int segment_size = right - left + 1;
    int threshold = get_adaptive_threshold(segment_size);
    if (segment_size <= threshold) {
        insertion_sort_double(arr, left, right);
        return;
    }
    
    int i_med_low = select_lower_pivot_double(arr, left, right, segment_size);
    double lowerPivot = arr[i_med_low];
    int i_med_high = select_upper_pivot_double(arr, left, right, segment_size);
    double upperPivot = arr[i_med_high];
    
    if (lowerPivot > upperPivot) {
        swap_double(&arr[i_med_low], &arr[i_med_high]);
        lowerPivot = arr[i_med_low];
        upperPivot = arr[i_med_high];
    }
    
    int l = left, r = right;
    for (int i = left; i <= r;) {
        if (arr[i] < lowerPivot) {
            swap_double(&arr[i], &arr[l]);
            l++;
            i++;
        } else if (arr[i] > upperPivot) {
            swap_double(&arr[i], &arr[r]);
            r--;
        } else {
            i++;
        }
    }
    
    if (l == left || r == right) {
        merge_sort_double(arr, left, right);
        return;
    }
    
    hybrid_min_max_sort_serial_double(arr, left, l - 1, k);
    hybrid_min_max_sort_serial_double(arr, l, r, k);
    hybrid_min_max_sort_serial_double(arr, r + 1, right, k);
}

