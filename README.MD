# Hybrid Min-Max Sort

Hybrid Min-Max Sort is an experimental sorting algorithm that combines ideas from QuickSort and MergeSort using two pivot elements. It was developed for educational purposes to explore hybrid sorting algorithms and performance optimization techniques.

---

## Description

This algorithm:
- Uses **two pivot elements** (lower and upper) to divide the array into three parts: less than the lower pivot, between the pivots, and greater than the upper pivot.
- Applies **clustering around medians** for more balanced partitioning.
- Includes **hybridization with MergeSort** in case of inefficient partitioning, ensuring O(n log n) performance in the worst case.
- **Optimized for both int and double** data types.

---

## Performance

The algorithm was tested against:
- **QuickSort (stdlib.h qsort)**
- To be added...

| Array Size     | Hybrid Min-Max Sort | QSort (stdlib.h) |
| -------------- | ------------------- | ---------------- |
| **1000**       | 0.000032 sec         | 0.000060 sec     | 
| **10000**      | 0.000430 sec         | 0.000750 sec     | 
| **100000**     | 0.005435 sec         | 0.009286 sec     | 
| **1000000**    | 0.065302 sec         | 0.113376 sec     |

- The algorithm is **faster than QSort** across all array sizes.
- On an **array of 1,000,000 elements**, the algorithm is approximately **42% faster** than QSort.
