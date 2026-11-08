# LangBench

## Layout

- `c/` and `lang/`: benchmark sources and shared input helpers.
- `darwin/c/` and `darwin/lang/`: macOS executables, moved from the source directories.
- `linux/c/` and `linux/lang/`: Linux executables.
- `scripts/`: native build and small-input correctness checks.
- `build/<platform>/`: ignored build logs, machine evidence and a JSON build report.

The preserved macOS binaries are arm64. The Linux binaries built here are
x86-64. Builds target the current host only; rebuilding on another architecture
replaces that platform's binaries. These folders do not imply cross-compilation.
Lang uses host CPU evidence, so rebuild on the target machine when its CPU or
native libraries differ. See [linux/README.md](linux/README.md) for build provenance.

Existing executable names are retained: Lang's fibers executable is
`fibers-dynamic`, and its sorted-k executable is `sorted_k`. Both `sorted-first`
and the historical `sorted_first` are retained; a new Lang build updates both
from `lang/sorted-first-cli.lang`. `recurrence-jump` is C-only.

## Build and check

Python 3 and a C11 compiler are required for C. The Lang build additionally needs
a built [Lang compiler](https://github.com/unktomi/Lang), Node.js, and the native
dependencies required by that compiler. Build the compiler separately with its
documented `npm run build` workflow. No toolchains are installed by these scripts.

From this checkout (the scripts also work from another working directory):

```sh
python3 scripts/build.py --implementation c
python3 scripts/smoke-test.py --implementation c

export LANG_TOOLCHAIN=/path/to/Lang
# Optional when clang is not on PATH:
# export LANG_CLANG=/path/to/clang
python3 scripts/build.py --implementation lang
python3 scripts/smoke-test.py --implementation lang
```

The builder chooses `darwin` or `linux` automatically. Omit `--implementation`
to build both languages. `--compiler /path/to/Lang` overrides `LANG_TOOLCHAIN`;
without either, the default is a sibling `../Lang` checkout. `CC` and `CFLAGS`
override the C compiler and flags (defaults: `cc`, `-O3 -std=c11 -Wall -Wextra`).
Lang uses its native default optimization and explicit host machine evidence,
with `--require-derived-the-term`. It never falls back to unoptimized builds.

Use repeatable `--benchmark NAME` options on either script for a subset, for example:

```sh
python3 scripts/build.py --implementation c --benchmark sorted-k
python3 scripts/smoke-test.py --implementation c --benchmark sorted-k
./linux/c/sorted-k 1 9 2 5
# On macOS: ./darwin/c/sorted-k 1 9 2 5
```

A failed build returns a nonzero status and records the failure under `build/`.
Compilation uses temporary outputs, so compiler errors do not replace previous
executables. Only test or time a workload after its build succeeds. Smoke tests
use independent integer oracles and small inputs,
including wraparound, generated input, sorting ranks and invalid arguments.
They do not run the expensive benchmark defaults or measure performance.

Builder regression tests: `python3 -m unittest discover -s tests -v`.

## Historical results

The original results below are retained. Moving the binaries and running Linux
correctness tests does not reproduce or update these timings. See
[comparison.md](comparison.md) for the original method.

| Workload | C -O3 ms | Lang ms | Result |
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
