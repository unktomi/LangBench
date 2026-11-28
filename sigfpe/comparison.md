# SIGFPE: Lang vs Haskell

Median whole-process runtime. Append `--repeat N` using the repetition count shown. Ratios above 1× favor Lang.

## macOS · Apple M2 Ultra

| Workload | Runtime input | Repetitions | Haskell (ms) | Lang (ms) | Haskell ÷ Lang | Rounds |
|---|---|---:|---:|---:|---:|---:|
| `running-totals` | `1 2 3 4` | 20,000,000 | 1,029.824 | 34.435 | 29.91× | 9 |
| `circular-blur` | `220 23 132 212 0` | 20,000,000 | 1,026.109 | 39.906 | 25.71× | 9 |
| `weighted-dictionary` | `135 -128 27 79 1 1` | 20,000,000 | 2,127.577 | 28.111 | 75.68× | 9 |
| `series-composition` | `43 -23 17 19` | 5,000,000 | 956.800 | 15.708 | 60.91× | 9 |
| `weather-paths` | `35 90 70 40 30 60 20 70` | 3,000,000 | 3,137.150 | 19.655 | 159.61× | 9 |
| `free-trace` | `2048 -1024 33 17` | 20,000,000 | 1,378.857 | 40.733 | 33.85× | 9 |
| `flag-flow` | `65536 32768 -127 0 1 0` | 20,000,000 | 658.605 | 21.173 | 31.11× | 9 |

## Linux · AMD EPYC 9V74

| Workload | Runtime input | Repetitions | Haskell (ms) | Lang (ms) | Haskell ÷ Lang | Rounds |
|---|---|---:|---:|---:|---:|---:|
| `running-totals` | `1 2 3 4` | 20,000,000 | 1,372.602 | 26.449 | 51.90× | 9 |
| `circular-blur` | `220 23 132 212 0` | 20,000,000 | 1,202.950 | 37.630 | 31.97× | 9 |
| `weighted-dictionary` | `135 -128 27 79 1 1` | 20,000,000 | 2,202.014 | 25.009 | 88.05× | 9 |
| `series-composition` | `43 -23 17 19` | 5,000,000 | 1,111.758 | 15.566 | 71.42× | 9 |
| `weather-paths` | `35 90 70 40 30 60 20 70` | 3,000,000 | 3,182.205 | 30.823 | 103.24× | 9 |
| `free-trace` | `2048 -1024 33 17` | 20,000,000 | 1,481.881 | 31.797 | 46.60× | 9 |
| `flag-flow` | `65536 32768 -127 0 1 0` | 20,000,000 | 761.613 | 24.674 | 30.87× | 9 |
