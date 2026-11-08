# Linux binaries

Built and checked on 2026-11-06 from:

- LangBench `main`: `a123124e4f30051cbc154e59c05fd8978bb4c7b9`.
- Lang `codex/compiler-ir-cleanup`: `fc3d8929ebb85719c2bb58feded4f127e6846075`.
- Debian 13, x86-64, Intel Xeon Platinum 8573C.
- GCC 14.2.0, Clang 19.1.7, Node.js 24.22.1.

`c/` contains 11 executables, including the C-only `recurrence-jump`.
`lang/` contains 10 workloads plus the `sorted_first` compatibility copy of
`sorted-first`. All files were verified as executable x86-64 ELF binaries.

The refreshed Lang compiler built from an upstream-hash-verified source snapshot
without local compiler patches. Lang workloads were built with explicit host
machine evidence and `--require-derived-the-term`; no unoptimized fallback was
used. C uses `-O3 -std=c11 -Wall -Wextra`.

Verification passed:

- All 11 C and 10 Lang workload builds.
- 438 independent C correctness and invalid-input checks.
- 400 independent Lang correctness and invalid-input checks.
- 10 builder routing and failure-handling regression tests.

The binaries are dynamically linked. Lang requires `libgmp.so.10` in addition
to the system C/math libraries. Lang's host evidence may select CPU-specific
instructions, so rebuild for a different CPU or system. Build logs and evidence
are generated under the ignored `build/linux/` directory; see the root
[README](../README.md) for reproduction commands.

These checks establish small-input correctness, not new benchmark timings.
The macOS binaries were preserved byte-for-byte but were not executed on Linux.
