# LangBench

Median runtime in ms, including startup. Matched runs use the same inputs and outputs. Ratios above 1× favor Lang. Lang uses LLVM unless otherwise noted.

## macOS · Apple M2 Ultra

| Workload | Runtime input | C -O3 (ms) | Lang (ms) | C -O3 ÷ Lang | Rounds |
|---|---|---:|---:|---:|---:|
| `ackermann` | `3 12` | 637.603 | 3.433 | 185.75× | 3 |
| `ackermann` | `3 13` | 2,562.560 | 3.590 | 713.73× | 3 |
| `gibonacci` | `2 3 45` | 4,142.670 | 4.005 | 1,034.47× | 3 |
| `hanoi-fixed` | `31` | 4,978.524 | 2.909 | 1,711.42× | 3 |
| `iterate` | `5000000000 3 5 7` | 4,835.788 | 3.089 | 1,565.30× | 3 |
| `pair-energy` | `--generate 131072 42` | 5,566.945 | 7.131 | 780.68× | 3 |
| `recurrence` | `44 2 3` | 4,677.453 | 3.974 | 1,177.01× | 3 |
| `sorted-first` | `--generate 240000 42` | 5,069.140 | 10.334 | 490.52× | 3 |
| `sorted_k` | `0 --generate 240000 42` | 4,952.880 | 9.740 | 508.53× | 3 |
| `sorted_k` | `120000 --generate 240000 42` | 5,045.532 | 63.398 | 79.59× | 3 |
| `sorted_k` | `239999 --generate 240000 42` | 5,093.499 | 9.832 | 518.04× | 3 |
| `string-length` | `65536` | 310.695 | 3.816 | 81.43× | 9 |
| `fibers-dynamic` | `8388608` | 29.699 | 5.635 | 5.27× | 21 |
| `inversion-count` | `--generate 131072 42` | 776.593 | 8.634 | 89.94× | 11 |

## Linux · AMD EPYC 9V74

| Workload | Runtime input | C -O3 (ms) | Lang (ms) | C -O3 ÷ Lang | Rounds |
|---|---|---:|---:|---:|---:|
| `ackermann` | `3 12` | 622.951 | 1.510 | 412.62× | 3 |
| `ackermann` | `3 13` | 2,497.455 | 1.765 | 1,415.02× | 3 |
| `gibonacci` | `2 3 45` | 3,501.832 | 2.528 | 1,385.45× | 3 |
| `hanoi-fixed` | `31` | 4,616.055 | 1.200 | 3,847.18× | 3 |
| `iterate` | `5000000000 3 5 7` | 5,727.144 | 1.030 | 5,558.05× | 3 |
| `pair-energy` | `--generate 131072 42` | 5,811.748 | 18.978 | 306.23× | 3 |
| `recurrence` | `44 2 3` | 4,481.506 | 1.430 | 3,133.42× | 3 |
| `sorted-first` | `--generate 240000 42` | 4,140.741 | 27.863 | 148.61× | 3 |
| `sorted_k` | `0 --generate 240000 42` | 4,320.325 | 14.857 | 290.80× | 5 |
| `sorted_k` | `120000 --generate 240000 42` | 4,283.853 | 70.439 | 60.82× | 5 |
| `sorted_k` | `239999 --generate 240000 42` | 4,158.141 | 15.047 | 276.34× | 5 |
| `string-length` | `65536` | 388.294 | 1.114 | 348.45× | 3 |
| `fibers-dynamic` | `8388608` | 284.420 | 8.633 | 32.95× | 3 |
| `inversion-count` | `--generate 131072 42` | 2,650.285 | 7.511 | 352.85× | 10 |
