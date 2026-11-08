These are the original reported results, not new Linux measurements. The
original macOS binaries now live in `darwin/c/` and `darwin/lang/`; native Linux
builds live in `linux/c/` and `linux/lang/`. See [README.md](README.md) for build
and correctness-check commands.

## Method

- Lang: `-O1`, explicit machine evidence, `--require-derived-the-term`; C: Clang `-O3 -std=c11`.
- 7 serial measured runs per executable after 1 warmup(s); alternating implementation order.
- One complete computation per process. Wall time includes startup, parsing/generation, allocation, output and teardown. No subtraction of startup, no repetition inside the executable.
- Independent integer oracles run outside timed intervals. Every warmup and measured output is also checked.
- Table entries are medians. C/Lang greater than one means Lang is faster. Millisecond-scale results can be startup dominated; these are not warmed-kernel timings.
- Host activity was recorded, not excluded by CPU isolation (load averages 5.18/4.26/3.12 before, 3.22/3.70/3.16 after). No CPU/allocation profiles were collected, so these ratios do not establish causal attribution.

## Results

| Workload | C O3 ms | Lang O1 ms | Result |
|---|---:|---:|---:|
| Ackermann(3,12) | 616.282 | 3.672 | 167.83× faster |
| Ackermann(3,13) | 2462.642 | 3.746 | 657.33× faster |
| Hanoi, 31 disks | 4811.564 | 3.305 | 1455.72× faster |
| Affine iteration, n=5,000,000,000 | 4626.568 | 3.269 | 1415.48× faster |
| Second-order recurrence, n=44 | 4381.705 | 3.170 | 1382.42× faster |
| Gibonacci depth 45, seeds 2/3 | 3869.860 | 3.422 | 1130.92× faster |
| Fibers, 8,388,608 rows | 28.858 | 5.780 | 4.99× faster |
| Recursive text construction, depth 65,536 | 283.268 | 2.927 | 96.76× faster |
| Sorted first, 240,000 values, seed 42 | 4770.485 | 13.625 | 350.12× faster |
| Sorted k, 240,000 values, seed 42 | 5.003s | 0.057s | 87× faster |
| Pair energy, 131,072 values (17,179,869,184 pair terms) | 5254.203 | 8.544 | 614.93× faster |

Measured wins: Lang 11/11 workloads.
