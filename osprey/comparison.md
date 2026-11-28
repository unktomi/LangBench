# Osprey

Median runtime in ms, including startup. Matched runs use the same inputs and outputs. Ratios above 1× favor Lang. Lang uses LLVM unless marked Rust.

## macOS · Apple M2 Ultra

| Workload | Runtime input | C -O3 (ms) | Lang (ms) | C -O3 ÷ Lang |
|---|---|---:|---:|---:|
| `ackermann` | `3 12` | 2,140.860 | 3.684 | 581.09× |
| `binarytrees` | `2500 13` | 983.534 | 3.653 | 269.23× |
| `coins` | `1100 5` | 1,635.888 | 3.689 | 443.50× |
| `collatz` | `5000000` | 722.230 | 749.650 | 0.96× |
| `coprime` | `11000` | 2,800.519 | 4.297 | 651.67× |
| `exprtree` | EOF on stdin, `--repeat 70` | 635.844 | 135.317 | 4.70× |
| `factorial` | `650000000` | 2,560.463 | 1,053.126 | 2.43× |
| `fib` | `45` | 3,139.215 | 2.954 | 1,062.68× |
| `hanoi` | `30` | 2,114.016 | 3.024 | 699.13× |
| `listops` | EOF on stdin, `--repeat 12000` | 743.607 | 73.130 | 10.17× |
| `mutual` | `25000000` | 2,767.298 | 15.735 | 175.87× |
| `nestedloop` | `1000` | 3,355.063 | 3.441 | 975.16× |
| `pascal` | `32 16` | 981.308 | 3.616 | 271.39× |
| `primes-observer` | `15000000` | 2,662.544 | 226.042 | 11.78× |
| `tak` | `7 0 44` | 636.583 | 3.797 | 167.65× |
| `textstats` | EOF on stdin, `--repeat 1000` | 1,181.680 | 9.502 | 124.36× |
| `wordfreq` | EOF on stdin, `--repeat 2200` | 744.840 | 5.983 | 124.50× |

## Linux · AMD EPYC 9V74

| Workload | Runtime input | C -O3 (ms) | Lang (ms) | C -O3 ÷ Lang |
|---|---|---:|---:|---:|
| `ackermann` | `3 12` | 653.137 | 1.619 | 403.30× |
| `binarytrees` | `2500 13` | 659.829 | 2.171 | 303.99× |
| `coins` | `1100 5` | 688.937 | 2.100 | 328.02× |
| `collatz` | `5000000` | 650.597 | 648.999 | 1.00× |
| `coprime` | `11000` | 3,072.236 | 2.602 | 1,180.75× |
| `exprtree` | EOF on stdin, `--repeat 70` | 610.090 | 104.286 | 5.85× |
| `factorial` | `650000000` | 2,703.662 | 1,250.490 | 2.16× |
| `fib` | `45` | 2,800.458 | 1.401 | 1,998.63× |
| `hanoi` | `30` | 1,635.161 | 1.521 | 1,074.95× |
| `listops` | EOF on stdin, `--repeat 12000` | 1,435.736 | 223.131 | 6.43× |
| `mutual` | `25000000` | 2,043.199 | 13.851 | 147.51× |
| `nestedloop` | `1000` | 3,402.475 | 1.214 | 2,801.98× |
| `pascal` | `32 16` | 1,035.238 | 2.112 | 490.17× |
| `primes-observer` | `15000000` | 5,382.130 | 455.225 | 11.82× |
| `tak` | `7 0 44` | 891.668 | 2.109 | 422.71× |
| `textstats` | EOF on stdin, `--repeat 1000` | 1,209.775 | 10.573 | 114.42× |
| `wordfreq` | EOF on stdin, `--repeat 2200` | 1,273.119 | 6.359 | 200.20× |

All inputs are supplied at runtime. The four stdin workloads use seed 1 at EOF; a valid nonzero signed decimal requests fresh entropy.

[Source](lang/) · [Upstream suite](https://github.com/korulang/koru-benchmarks/tree/38a49bc578b5f6d3fa12939bc69aec9cbd6f468b/suites/osprey-compute-kernels)
