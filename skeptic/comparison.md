# Skeptic benchmarks

Median runtime in ms, including startup. Matched runs use the same inputs and outputs. Ratios above 1× favor Lang.

## macOS · Apple M2 Ultra

| Workload | Runtime input | C -O3 (ms) | Lang (ms) | C -O3 ÷ Lang | Rounds |
|---|---|---:|---:|---:|---:|
| `static-vm` | `160000000 42` | 676.011 | 675.044 | 1.00× | 7 |
| `lcs` | `20 --generate 40 42` | 523.685 | 3.431 | 152.63× | 7 |
| `convolution` | `49152 --generate 98304 42` | 782.548 | 203.389 | 3.85× | 7 |
| `convolution` | `49152 --generate 98304 42` | 782.548 | 193.400 | 4.05× | 7 |
| `convolution` | `65536 --generate 131072 42` | 1348.909 | 207.623 | 6.50× | 7 |
| `convolution` | `65536 --generate 131072 42` | 1348.909 | 195.567 | 6.90× | 7 |
| `embedding-pairs` | `12 --generate 24 42` | 535.986 | 4.396 | 121.94× | 7 |
| `weighted-alignment` | `12 --generate 24 43` | 919.657 | 4.451 | 206.63× | 7 |

## Linux · AMD EPYC 9V74

| Workload | Runtime input | C -O3 (ms) | Lang (ms) | C -O3 ÷ Lang | Rounds |
|---|---|---:|---:|---:|---:|
| `static-vm` | `160000000 42` | 646.728 | 646.325 | 1.00× | 7 |
| `lcs` | `20 --generate 40 42` | 624.936 | 2.375 | 263.16× | 7 |
| `convolution` | `49152 --generate 98304 42` | 1080.078 | 256.738 | 4.21× | 7 |
| `convolution` | `49152 --generate 98304 42` | 1080.078 | 266.670 | 4.05× | 7 |
| `convolution` | `65536 --generate 131072 42` | 1892.326 | 247.037 | 7.66× | 7 |
| `convolution` | `65536 --generate 131072 42` | 1892.326 | 274.924 | 6.88× | 7 |
| `embedding-pairs` | `12 --generate 24 42` | 610.461 | 2.325 | 262.59× | 7 |
| `weighted-alignment` | `12 --generate 24 43` | 982.442 | 2.428 | 404.58× | 7 |
