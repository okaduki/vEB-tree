# Van Emde Boas Tree

## References

* Introduction to Algorithms, 3rd Edition (Chapter 20)
* Reduced Space vEBTree(RSvEBTree) is introduced in Problem 20.1

## Benchmark

* command: `make && ./vebtree_test bench`

* Compiler: g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
* Arch: Windows10 64bit, Intel(R) Core(TM) i5-8500 CPU
* RAM: 16GB

* 1000000 operations (0 <= key < 2^20)

| Algorithm | insert | lookup (almost miss) | lookup (always hit) | upper_bound | erase |
| --------- | ------ | -------------------- | ------------------- | ----------- | ----- |
| Set (STL) | 697    | 751                  | 828                 | 705         | 661   |
| vEBTree   | 398    | 333                  | 407                 | 390         | 399   |
| RSvEBTree | 1202   | 821                  | 887                 | 989         | 1100  |

* 10000000 operations  (0 <= key < 2^40)

| Algorithm | insert | lookup (almost miss) | lookup (always hit) | upper_bound | erase |
| --------- | ------ | -------------------- | ------------------- | ----------- | ----- |
| Set (STL) | 111749 | 12529                | 13550               | 12348       | 11833 |
| RSvEBTree | 25183  | 4121                 | 5770                | 17286       | 17827 |
