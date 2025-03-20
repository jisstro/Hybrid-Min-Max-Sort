# Hybrid Min-Max Sort

Hybrid Min-Max Sort is an experimental sorting algorithm that combines ideas from QuickSort and MergeSort using two pivot elements. It was developed for educational purposes to explore hybrid sorting algorithms and performance optimization techniques. 

---
## Description

This algorithm:
- Uses **two pivot elements** (lower and upper) to divide the array into three parts: less than the lower pivot, between the pivots, and greater than the upper pivot.
- Applies **clustering around medians** for more balanced partitioning.
- Includes **hybridization with MergeSort** in case of inefficient partitioning, ensuring O(n log n) performance in the worst case.
- **Optimized for both int and double** data types. (in C, in c++ only int)
---

##Adaptive Partitioning:
Threshold-Based Sorting:

If the segment size is below a predefined threshold, Insertion Sort is applied for optimal performance on small arrays.
For larger segments, the array is partitioned using two pivots:
Lower Pivot: Chosen as the median of the first few elements.
Upper Pivot: Chosen as the median of the last few elements.
Three-Way Partitioning:

The array is divided into three distinct parts:
Elements less than the lower pivot.
Elements between the two pivots.
Elements greater than the upper pivot.
Hybridization with MergeSort:
If the partitioning is inefficient (i.e., the segments are not reducing in size), the algorithm switches to MergeSort.
This hybrid approach ensures stable performance even in cases where QuickSort would degrade to O(n²).
Recursive Sorting:
The algorithm recursively sorts each of the three partitions:
Elements smaller than the lower pivot.
Elements between the two pivots.
Elements larger than the upper pivot.
Tail recursion optimization and adaptive thresholds are used to enhance performance and reduce stack depth.

##Asymptotic Analysis of Hybrid Min-Max Sort:
Time Complexity:
Best Case: O(n log n)

Occurs when pivot elements evenly divide the array into three parts.
In this case, the recursion depth is minimized, and the number of comparisons and swaps is optimal.
Average Case: O(n log n)

The use of medians from multiple elements ensures a high probability of balanced partitioning.
Recursive calls handle roughly equal subarrays, maintaining a logarithmic recursion depth.
Worst Case: O(n log n)

In the worst case, the algorithm hybridizes with MergeSort, which guarantees O(n log n) complexity.
This avoids the performance degradation typically seen in QuickSort (O(n²) in the worst case).
Space Complexity:
O(n) — for the temporary buffer used in MergeSort.
O(log n) — for the recursion stack in the optimized version.

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

## License
This project is licensed under the GPL v3. See the LICENSE file for more details.

## Contact
If you have any questions or suggestions, feel free to contact through GitHub Issues or reach out at simatsan186@gmail.com.

##Compilation and Usage

To compile and run the algorithm, use the following commands:

```bash
gcc -O3 main.c min_max_sort.c -o min_max_sort
./min_max_sort

