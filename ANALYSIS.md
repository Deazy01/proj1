# Deque Analysis

## Data

The following results were collected from `bin/analysis` using `--max 256` with 1024, 2048, 4096, and 8192 iterations.

|       | Max    |         |         |          | Var    |         |         |         |
| ----- | ------ | ------- | ------- | -------- | ------ | ------- | ------- | ------- |
| Total | 1024   | 2048    | 4096    | 8192     | 1024   | 2048    | 4096    | 8192    |
| PushF | 519908 | 1179780 | 2142420 | 4752580  | 626912 | 1080930 | 2112320 | 4679170 |
| PushB | 494783 | 775779  | 1595890 | 3805160  | 415606 | 758770  | 1590090 | 3869370 |
| PopF  | 345638 | 711047  | 1325120 | 3159090  | 350042 | 673829  | 1311920 | 3324010 |
| PopB  | 300894 | 789986  | 1154680 | 2895950  | 338931 | 704044  | 1155980 | 3104180 |
| Forw  | 796478 | 1927240 | 2808690 | 7378600  | 897176 | 1854410 | 2836590 | 8472730 |
| Rev   | 866346 | 1881820 | 3026840 | 10370800 | 900279 | 1981970 | 2988730 | 8090090 |

|       | Max     |         |         |         | Var     |         |         |         |
| ----- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- |
| Mean  | 1024    | 2048    | 4096    | 8192    | 1024    | 2048    | 4096    | 8192    |
| PushF | 507.722 | 576.063 | 523.053 | 580.149 | 612.219 | 527.797 | 515.702 | 571.188 |
| PushB | 483.187 | 378.798 | 389.621 | 464.498 | 405.865 | 370.493 | 388.205 | 472.335 |
| PopF  | 337.537 | 347.191 | 323.516 | 385.631 | 341.838 | 329.018 | 320.293 | 405.762 |
| PopB  | 293.842 | 385.735 | 281.905 | 353.510 | 330.987 | 343.771 | 282.222 | 378.928 |
| Forw  | 777.810 | 941.036 | 685.714 | 900.708 | 876.149 | 905.471 | 692.527 | 1034.27 |
| Rev   | 846.041 | 918.857 | 738.974 | 1265.96 | 879.179 | 967.758 | 729.670 | 987.560 |

|        | Max     |         |         |         | Var     |         |         |         |
| ------ | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- |
| StdDev | 1024    | 2048    | 4096    | 8192    | 1024    | 2048    | 4096    | 8192    |
| PushF  | 789.564 | 1410.44 | 1372.46 | 1197.25 | 1440.18 | 1453.82 | 1292.80 | 772.401 |
| PushB  | 1428.63 | 46.0445 | 904.906 | 842.917 | 155.715 | 46.0337 | 465.257 | 1136.29 |
| PopF   | 728.634 | 1031.60 | 463.591 | 628.886 | 49.527  | 491.648 | 275.222 | 717.714 |
| PopB   | 47.7367 | 2134.21 | 39.8077 | 656.440 | 46.2441 | 50.3921 | 266.128 | 2053.76 |
| Forw   | 42.7122 | 3441.20 | 1029.74 | 502.242 | 1855.90 | 3689.96 | 1011.19 | 1115.84 |
| Rev    | 49.843  | 50.5569 | 860.979 | 1498.28 | 936.115 | 2236.98 | 801.053 | 1049.39 |

## Analysis

### What seems to be the growth rate for PushF, PushB, PopF, etc. for your Max and Var implementations?

The total execution times generally grow approximately linearly as the number of iterations increases. When the iterations are doubled, the total values generally increase by about two times, although there is some variation caused by system timing and measurement overhead.

PushF, PushB, PopF, and PopB all have approximately linear growth for both the Max and Var implementations. Forward and Reverse also show approximately linear growth because they process the elements in the deque.

The mean values remain in a relatively similar range as the number of iterations increases, which also supports approximately constant-time deque operations.

### Do they appear to grow faster or slower than expected?

The implementations generally grow at the expected rate. PushFront, PushBack, PopFront, and PopBack are expected to be approximately O(1), and the results are consistent with that because their mean execution times remain relatively stable.

Forward and Reverse perform more work as the number of elements increases, so their total execution times are larger. Their growth is also generally consistent with the expected behavior of operations that traverse the deque.

### If your implementation is deviating, why do you believe your implementation is deviating from what might be expected?

There is some variation between measurements, especially in the standard deviation values. This is expected in benchmark results because execution time can be affected by the operating system, CPU scheduling, cache behavior, memory allocation, and other processes running on the system.

The Var implementation can also have occasional extra work when its storage grows. These resize operations can cause some benchmark variation even though the overall behavior remains approximately linear.

### Why do you believe your Max and Var implementations differ in performance?

The Max implementation allocates its storage once at construction with a fixed capacity of 256. It therefore does not need to resize its storage during the benchmark.

The Var implementation starts with a smaller capacity and increases its capacity when it becomes full. Growing requires allocating a larger array and moving the existing elements, so this can introduce additional overhead.

Both implementations use a circular-array design, so normal PushFront, PushBack, PopFront, and PopBack operations are efficient. The main performance difference comes from the additional resizing work required by the Var implementation.
