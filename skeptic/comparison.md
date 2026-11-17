# Skeptic benchmarks

| Workload | Arguments | TypeScript (ms) | Lang (ms) |
| --- | --- | ---: | ---: |
| Static VM | `20000000 42` | 2,208.01 | 84.31 |
| LCS | `18 --generate 36 42` | 251.49 | 3.46 |
| Convolution | `8192 --generate 16384 42` | 1,867.42 | 37.02 |

Apple M2 Ultra. Median of five CLI runs including startup. TypeScript used Node v26.12.1 and Lang used native LLVM at -O1. Both returned the same result in every run.
