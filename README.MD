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
##Algorithm Explanation
Adaptive Partitioning:

If the segment is smaller than a certain threshold, insertion sort is used for better performance.
For larger segments, two pivot elements are selected:
Lower pivot — median of the first few elements.
Upper pivot — median of the last few elements.
The array is divided into three parts:
Less than the lower pivot.
Between the two pivots.
Greater than the upper pivot.
Hybridization with MergeSort:
If partitioning is inefficient (segments are not reducing in size), MergeSort is used.
Recursive Sorting:
The algorithm recursively applies itself to each of the three segments.
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

## Contributing and Feedback
This project is open to constructive feedback and suggestions for improvement. If you have ideas on how to enhance its performance or code quality, feel free to open an Issue or submit a Pull Request.

## License
This project is licensed under the GPL v3. See the LICENSE file for more details.

## Acknowledgments
This project was inspired by the curiosity to explore algorithms and improve performance. Special thanks to the open-source community for their support and inspiration.

## Contact
If you have any questions or suggestions, feel free to contact through GitHub Issues or reach out at simatsan186@gmail.com.

This project is part of [Sorting Algorithms](https://github.com/topics/sorting-algorithm) on GitHub.

##Compilation and Usage

To compile and run the algorithm, use the following commands:

```bash
gcc -O3 main.c min_max_sort.c -o min_max_sort
./min_max_sort

