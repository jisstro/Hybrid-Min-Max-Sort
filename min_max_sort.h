/*
 * min_max_sort.h
 *
 * Заголовочный файл для гибридной сортировки Min-Max для типов int и double.
 */

#ifndef MIN_MAX_SORT_H
#define MIN_MAX_SORT_H

/* --- Прототипы функций для int --- */
void insertion_sort(int arr[], int low, int high);
int get_adaptive_threshold(int segment_size);
int median_of_five_index(const int arr[], int i1, int i2, int i3, int i4, int i5);
int median_of_three_index(const int arr[], int i1, int i2, int i3);
int select_lower_pivot(const int arr[], int left, int right, int segment_size);
int select_upper_pivot(const int arr[], int left, int right, int segment_size);
void merge(int arr[], int left, int mid, int right);
void merge_sort(int arr[], int left, int right);
void hybrid_min_max_sort_serial(int arr[], int left, int right, int k);

/* --- Прототипы функций для double --- */
void insertion_sort_double(double arr[], int low, int high);
int median_of_five_index_double(const double arr[], int i1, int i2, int i3, int i4, int i5);
int median_of_three_index_double(const double arr[], int i1, int i2, int i3);
int select_lower_pivot_double(const double arr[], int left, int right, int segment_size);
int select_upper_pivot_double(const double arr[], int left, int right, int segment_size);
void merge_double(double arr[], int left, int mid, int right);
void merge_sort_double(double arr[], int left, int right);
void hybrid_min_max_sort_serial_double(double arr[], int left, int right, int k);

#endif /* MIN_MAX_SORT_H */

