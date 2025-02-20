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
##Compilation and Usage

To compile and run the algorithm, use the following commands:

```bash
gcc -O3 main.c min_max_sort.c -o min_max_sort
./min_max_sort

## Contributing and Feedback
This project is open to constructive feedback and suggestions for improvement. If you have ideas on how to enhance its performance or code quality, feel free to open an Issue or submit a Pull Request.

## License
This project is licensed under the GPL v3. See the LICENSE file for more details.

## Acknowledgments
This project was inspired by the curiosity to explore algorithms and improve performance. Special thanks to the open-source community for their support and inspiration.

## Contact
If you have any questions or suggestions, feel free to contact through GitHub Issues or reach out at simatsan186@gmail.com.

