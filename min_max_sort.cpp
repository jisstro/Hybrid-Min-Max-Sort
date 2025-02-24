/*
 * min_max_sort.cpp
 *
 * Реализация гибридной сортировки Min-Max для типов int и double на C++.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <chrono> // Для замера времени

#define THRESHOLD_DEFAULT 64
#define SMALL_SIZE 128

/* ==================== Вспомогательные функции ==================== */

// Обмен значений (универсальный шаблон)
template <typename T>
inline void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/* ==================== Функции сортировки для int ==================== */

// Сортировка вставками для int
void insertion_sort(std::vector<int>& arr, int low, int high) {
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

// Адаптивный порог
int get_adaptive_threshold(int segment_size) {
    return (segment_size < THRESHOLD_DEFAULT) ? segment_size : THRESHOLD_DEFAULT;
}

// Быстрая медиана из пяти элементов с использованием std::nth_element
int median_of_five_index(const std::vector<int>& arr, int i1, int i2, int i3, int i4, int i5) {
    std::vector<int> indices = { i1, i2, i3, i4, i5 };
    std::vector<int> values = { arr[i1], arr[i2], arr[i3], arr[i4], arr[i5] };
    std::nth_element(values.begin(), values.begin() + 2, values.end());
    int median_value = values[2];
    for (int idx : indices) {
        if (arr[idx] == median_value) return idx;
    }
    return i3; // На случай дублирующихся элементов
}

// Медиана из 3 элементов (возвращает индекс)
int median_of_three_index(const std::vector<int>& arr, int i1, int i2, int i3) {
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
int select_lower_pivot(const std::vector<int>& arr, int left, int right, int segment_size) {
    if (segment_size < SMALL_SIZE) {
        return median_of_three_index(arr, left, left + segment_size / 4, left + segment_size / 2);
    } else {
        return median_of_five_index(arr, left, left + segment_size / 8, left + segment_size / 4,
                                     left + (3 * segment_size) / 8, left + segment_size / 2);
    }
}

// Выбор верхнего опорного элемента (для правого сегмента)
int select_upper_pivot(const std::vector<int>& arr, int left, int right, int segment_size) {
    if (segment_size < SMALL_SIZE) {
        return median_of_three_index(arr, left + segment_size / 2, left + (3 * segment_size) / 4, right);
    } else {
        return median_of_five_index(arr, left + segment_size / 2, left + (5 * segment_size) / 8,
                                     left + (3 * segment_size) / 4, left + (7 * segment_size) / 8, right);
    }
}

// Классическое слияние двух отсортированных частей массива
void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (int l = 0; l < k; l++) {
        arr[left + l] = temp[l];
    }
}

// Классическая сортировка слиянием
void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Гибридная сортировка для int
void hybrid_min_max_sort(std::vector<int>& arr, int left, int right, int k) {
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
        swap(arr[i_med_low], arr[i_med_high]);
        lowerPivot = arr[i_med_low];
        upperPivot = arr[i_med_high];
    }
    
    int l = left, r = right;
    for (int i = left; i <= r;) {
        if (arr[i] < lowerPivot) {
            swap(arr[i], arr[l]);
            l++;
            i++;
        } else if (arr[i] > upperPivot) {
            swap(arr[i], arr[r]);
            r--;
        } else {
            i++;
        }
    }
    
    if (l == left || r == right) {
        merge_sort(arr, left, right);
        return;
    }
    
    hybrid_min_max_sort(arr, left, l - 1, k);
    hybrid_min_max_sort(arr, l, r, k);
    hybrid_min_max_sort(arr, r + 1, right, k);
}

int main() {
    std::vector<int> arr = {3, 6, 8, 10, 1, 2, 1, 5, 9, 4, 7, 9, 19, 284, 20, 21, 384, 39, 29, 0, 1, -34, 23};
    int k = 2;

    auto start = std::chrono::high_resolution_clock::now();
    hybrid_min_max_sort(arr, 0, arr.size() - 1, k);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Отсортированный массив: ";
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;

    return 0;
}

