#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <cstring>

// You can change this block size for experiments.
constexpr int BLOCK_SIZE = 1048;
constexpr int INSERTION_SORT_THRESHOLD = 32;

// ------------------ Functions for int ------------------

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

int get_adaptive_threshold(int segment_size) {
    return (segment_size < 64) ? segment_size : 64;
}

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

int get_max(const int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

void counting_sort_for_radix(int arr[], int n, int exp) {
    std::vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

int select_lower_pivot(const int arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        int i1 = left;
        int i2 = left + segment_size / 4;
        int i3 = left + segment_size / 2;
        return median_of_three_index(arr, i1, i2, i3);
    } else {
        int i1 = left;
        int i2 = left + segment_size / 8;
        int i3 = left + segment_size / 4;
        int i4 = left + (3 * segment_size) / 8;
        int i5 = left + segment_size / 2;
        return median_of_five_index(arr, i1, i2, i3, i4, i5);
    }
}

int select_upper_pivot(const int arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        int i1 = left + segment_size / 2;
        int i2 = left + (3 * segment_size) / 4;
        int i3 = right;
        return median_of_three_index(arr, i1, i2, i3);
    } else {
        int i1 = left + segment_size / 2;
        int i2 = left + (5 * segment_size) / 8;
        int i3 = left + (3 * segment_size) / 4;
        int i4 = left + (7 * segment_size) / 8;
        int i5 = right;
        return median_of_five_index(arr, i1, i2, i3, i4, i5);
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

// Оптимизированная сортировка слиянием (используется как fallback)
// Шаблонные функции: insertion_sort_opt, merge_opt, merge_sort_opt
template <typename T>
void insertion_sort_opt(T* arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template <typename T>
void merge_opt(T* arr, int left, int mid, int right, std::vector<T>& buffer) {
    int i = left, j = mid + 1, k = left;
    for (int idx = left; idx <= right; idx++) {
        buffer[idx] = arr[idx];
    }
    while (i <= mid && j <= right) {
        if (buffer[i] <= buffer[j])
            arr[k++] = buffer[i++];
        else
            arr[k++] = buffer[j++];
    }
    while (i <= mid)
        arr[k++] = buffer[i++];
    // Остаток правой части уже на месте
}

template <typename T>
void merge_sort_opt(T* arr, int n) {
    std::vector<T> buffer(n);
    // Сортируем мелкие блоки вставками
    for (int i = 0; i < n; i += INSERTION_SORT_THRESHOLD) {
        int right = std::min(i + INSERTION_SORT_THRESHOLD - 1, n - 1);
        insertion_sort_opt(arr, i, right);
    }
    // Итеративное объединение блоков
    for (int step = INSERTION_SORT_THRESHOLD; step < n; step *= 2) {
        for (int left = 0; left < n - step; left += 2 * step) {
            int mid = left + step - 1;
            int right = std::min(left + 2 * step - 1, n - 1);
            merge_opt(arr, left, mid, right, buffer);
        }
    }
}

// Гибридная сортировка для int с использованием оптимизированной merge sort как fallback
void hybrid_min_max_sort_serial(int arr[], int left, int right, int /*k*/) {
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
        std::swap(arr[i_med_low], arr[i_med_high]);
        lowerPivot = arr[i_med_low];
        upperPivot = arr[i_med_high];
    }
    
    int l = left, r = right;
    for (int i = left; i <= r;) {
        if (arr[i] < lowerPivot) {
            std::swap(arr[i], arr[l]);
            l++;
            i++;
        } else if (arr[i] > upperPivot) {
            std::swap(arr[i], arr[r]);
            r--;
        } else {
            i++;
        }
    }
    
    // Если разбиение оказалось неэффективным, используем оптимизированную merge sort
    if (l == left || r == right) {
        merge_sort_opt(arr + left, right - left + 1);
        return;
    }
    
    hybrid_min_max_sort_serial(arr, left, l - 1, 2);
    hybrid_min_max_sort_serial(arr, l, r, 2);
    hybrid_min_max_sort_serial(arr, r + 1, right, 2);
}

void copy_array(const int src[], int dest[], int n) {
    std::memcpy(dest, src, n * sizeof(int));
}

void generate_random_array(int arr[], int n, int max_value) {
    for (int i = 0; i < n; i++) {
        arr[i] = std::rand() % max_value;
    }
}

bool is_sorted(const int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1])
            return false;
    }
    return true;
}

int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void radix_sort(int arr[], int n) {
    int max = get_max(arr, n);
    for (int exp = 1; max / exp > 0; exp *= 10)
        counting_sort_for_radix(arr, n, exp);
}

void test_hybrid_sort_int(int n) {
    std::vector<int> original(n);
    std::vector<int> arrHybrid(n);
    std::vector<int> arrQSort(n);
    std::vector<int> arrRadix(n);
    std::vector<int> arrStd(n);
    
    generate_random_array(original.data(), n, n * 10);
    copy_array(original.data(), arrHybrid.data(), n);
    copy_array(original.data(), arrQSort.data(), n);
    copy_array(original.data(), arrRadix.data(), n);
    copy_array(original.data(), arrStd.data(), n);
    
    auto start = std::chrono::high_resolution_clock::now();
    hybrid_min_max_sort_serial(arrHybrid.data(), 0, n - 1, 2);
    auto end = std::chrono::high_resolution_clock::now();
    double timeHybrid = std::chrono::duration<double>(end - start).count();
    
    auto start_qsort = std::chrono::high_resolution_clock::now();
    std::qsort(arrQSort.data(), n, sizeof(int), compare_int);
    auto end_qsort = std::chrono::high_resolution_clock::now();
    double timeQSort = std::chrono::duration<double>(end_qsort - start_qsort).count();
    
    auto start_radix = std::chrono::high_resolution_clock::now();
    radix_sort(arrRadix.data(), n);
    auto end_radix = std::chrono::high_resolution_clock::now();
    double timeRadix = std::chrono::duration<double>(end_radix - start_radix).count();
    
    auto start_std = std::chrono::high_resolution_clock::now();
    std::sort(arrStd.begin(), arrStd.end());
    auto end_std = std::chrono::high_resolution_clock::now();
    double timeStd = std::chrono::duration<double>(end_std - start_std).count();
    
    std::cout << "===== Тест для int, размер массива: " << n << " =====\n";
    std::cout << "Hybrid Min-Max Sort: " << timeHybrid << " сек, Отсортировано: " 
              << (is_sorted(arrHybrid.data(), n) ? "ДА" : "НЕТ") << "\n";
    std::cout << "QSort (stdlib): " << timeQSort << " сек, Отсортировано: " 
              << (is_sorted(arrQSort.data(), n) ? "ДА" : "НЕТ") << "\n";
    std::cout << "Radix Sort: " << timeRadix << " сек, Отсортировано: " 
              << (is_sorted(arrRadix.data(), n) ? "ДА" : "НЕТ") << "\n";
    std::cout << "std::sort: " << timeStd << " сек, Отсортировано: " 
              << (is_sorted(arrStd.data(), n) ? "ДА" : "НЕТ") << "\n\n";
}

// ------------------ Functions for double ------------------

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

int select_lower_pivot_double(const double arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        int i1 = left;
        int i2 = left + segment_size / 4;
        int i3 = left + segment_size / 2;
        return median_of_three_index_double(arr, i1, i2, i3);
    } else {
        int i1 = left;
        int i2 = left + segment_size / 8;
        int i3 = left + segment_size / 4;
        int i4 = left + (3 * segment_size) / 8;
        int i5 = left + segment_size / 2;
        return median_of_five_index_double(arr, i1, i2, i3, i4, i5);
    }
}

int select_upper_pivot_double(const double arr[], int left, int right, int segment_size) {
    if (segment_size < 128) {
        int i1 = left + segment_size / 2;
        int i2 = left + (3 * segment_size) / 4;
        int i3 = right;
        return median_of_three_index_double(arr, i1, i2, i3);
    } else {
        int i1 = left + segment_size / 2;
        int i2 = left + (5 * segment_size) / 8;
        int i3 = left + (3 * segment_size) / 4;
        int i4 = left + (7 * segment_size) / 8;
        int i5 = right;
        return median_of_five_index_double(arr, i1, i2, i3, i4, i5);
    }
}

void merge_double(double arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    std::vector<double> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

// Гибридная сортировка для double с использованием оптимизированной merge sort при fallback
void hybrid_min_max_sort_serial_double(double arr[], int left, int right, int /*k*/) {
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
        std::swap(arr[i_med_low], arr[i_med_high]);
        lowerPivot = arr[i_med_low];
        upperPivot = arr[i_med_high];
    }
    
    int l = left, r = right;
    for (int i = left; i <= r;) {
        if (arr[i] < lowerPivot) {
            std::swap(arr[i], arr[l]);
            l++;
            i++;
        } else if (arr[i] > upperPivot) {
            std::swap(arr[i], arr[r]);
            r--;
        } else {
            i++;
        }
    }
    
    if (l == left || r == right) {
        merge_sort_opt(arr + left, right - left + 1);
        return;
    }
    
    hybrid_min_max_sort_serial_double(arr, left, l - 1, 2);
    hybrid_min_max_sort_serial_double(arr, l, r, 2);
    hybrid_min_max_sort_serial_double(arr, r + 1, right, 2);
}

void copy_array_double(const double src[], double dest[], int n) {
    std::memcpy(dest, src, n * sizeof(double));
}

void generate_random_array_double(double arr[], int n, double max_value) {
    for (int i = 0; i < n; i++) {
        arr[i] = (static_cast<double>(std::rand()) / RAND_MAX) * max_value;
    }
}

bool is_sorted_double(const double arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1])
            return false;
    }
    return true;
}

int compare_double(const void *a, const void *b) {
    double diff = (*(double*)a - *(double*)b);
    return (diff < 0) ? -1 : (diff > 0) ? 1 : 0;
}

void test_hybrid_sort_double(int n) {
    std::vector<double> original(n);
    std::vector<double> arrHybrid(n);
    std::vector<double> arrQSort(n);
    std::vector<double> arrStd(n);
    
    generate_random_array_double(original.data(), n, n * 10.0);
    copy_array_double(original.data(), arrHybrid.data(), n);
    copy_array_double(original.data(), arrQSort.data(), n);
    copy_array_double(original.data(), arrStd.data(), n);
    
    auto start = std::chrono::high_resolution_clock::now();
    hybrid_min_max_sort_serial_double(arrHybrid.data(), 0, n - 1, 2);
    auto end = std::chrono::high_resolution_clock::now();
    double timeHybrid = std::chrono::duration<double>(end - start).count();
    
    auto start_qsort = std::chrono::high_resolution_clock::now();
    std::qsort(arrQSort.data(), n, sizeof(double), compare_double);
    auto end_qsort = std::chrono::high_resolution_clock::now();
    double timeQSort = std::chrono::duration<double>(end_qsort - start_qsort).count();
    
    auto start_std = std::chrono::high_resolution_clock::now();
    std::sort(arrStd.begin(), arrStd.end());
    auto end_std = std::chrono::high_resolution_clock::now();
    double timeStd = std::chrono::duration<double>(end_std - start_std).count();
    
    std::cout << "===== Тест для double, размер массива: " << n << " =====\n";
    std::cout << "Hybrid Min-Max Sort: " << timeHybrid << " сек, Отсортировано: " 
              << (is_sorted_double(arrHybrid.data(), n) ? "ДА" : "НЕТ") << "\n";
    std::cout << "QSort (stdlib): " << timeQSort << " сек, Отсортировано: " 
              << (is_sorted_double(arrQSort.data(), n) ? "ДА" : "НЕТ") << "\n";
    std::cout << "std::sort: " << timeStd << " сек, Отсортировано: " 
              << (is_sorted_double(arrStd.data(), n) ? "ДА" : "НЕТ") << "\n\n";
}

// ------------------ main ------------------

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    // Тесты для int
    int sizes_int[] = {1000, 10000, 50000, 100000, 1000000, 10000000, 100000000};
    int num_sizes_int = sizeof(sizes_int) / sizeof(sizes_int[0]);
    for (int i = 0; i < num_sizes_int; i++) {
        test_hybrid_sort_int(sizes_int[i]);
    }
    
    // Тесты для double
    int sizes_double[] = {1000, 10000, 50000, 100000, 1000000, 10000000};
    int num_sizes_double = sizeof(sizes_double) / sizeof(sizes_double[0]);
    for (int i = 0; i < num_sizes_double; i++) {
        test_hybrid_sort_double(sizes_double[i]);
    }
    
    return 0;
}

