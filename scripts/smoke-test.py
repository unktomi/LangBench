#!/usr/bin/env python3
"""Independent small-input numerical oracles; never run costly default cases."""
import argparse
import collections
import functools
import platform
from pathlib import Path
import random
import subprocess

ROOT = Path(__file__).resolve().parent.parent
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--implementation', choices=('c', 'lang'), default='c')
parser.add_argument('--benchmark', action='append', choices=(
    'ackermann', 'fibers', 'gibonacci', 'hanoi-fixed', 'iterate', 'pair-energy',
    'recurrence', 'recurrence-jump', 'sorted-first', 'sorted-k', 'string-length'))
args = parser.parse_args()
host = platform.system().lower()
if host not in ('darwin', 'linux'):
    parser.error('Only native macOS and Linux binaries are supported')
if args.implementation == 'lang' and args.benchmark and 'recurrence-jump' in args.benchmark:
    parser.error('recurrence-jump is a C-only workload')
BIN = ROOT / host / args.implementation
LANG_NAMES = {'fibers': 'fibers-dynamic', 'sorted-k': 'sorted_k'}
MASK = (1 << 64) - 1
cli_args = args
counts = collections.Counter()

def check(name, args, expected=None, code=0):
    if cli_args.benchmark and name not in cli_args.benchmark:
        return
    if cli_args.implementation == 'lang' and name == 'recurrence-jump':
        return
    filename = LANG_NAMES.get(name, name) if cli_args.implementation == 'lang' else name
    executable = BIN / filename
    if not executable.is_file():
        raise SystemExit(f'Missing {executable}; run scripts/build.py --implementation {cli_args.implementation}')
    p = subprocess.run([str(executable), *map(str, args)], text=True,
                       capture_output=True, timeout=10)
    if p.returncode != code:
        raise SystemExit(f'FAIL {name} {args}: exit {p.returncode}, expected {code}; {p.stderr}')
    if code == 0 and p.stdout != f'{expected}\n':
        raise SystemExit(f'FAIL {name} {args}: output {p.stdout!r}, expected {expected!r}')
    counts[name] += 1

@functools.lru_cache(None)
def ack(m, n):
    if m == 0: return n + 1
    if n == 0: return ack(m - 1, 1)
    return ack(m - 1, ack(m, n - 1))

for m in range(3):
    for n in range(7): check('ackermann', [m, n], ack(m, n))
for n in range(4): check('ackermann', [3, n], (1 << (n + 3)) - 3)
check('ackermann', [0, MASK], 0)

rng = random.Random(42)
for n in [0, 1, 2, 3, 10, 15, 20]:
    check('hanoi-fixed', [n], (1 << n) - 1)
    for a, b in [(2, 3), (0, 0), (MASK, MASK - 1),
                 (rng.getrandbits(64), rng.getrandbits(64))]:
        seq = [a, b]
        for k in range(2, n + 1): seq.append((seq[-1] + seq[-2]) & MASK)
        check('gibonacci', [a, b, n], seq[n])
        seq = [a, b]
        for k in range(2, n + 1): seq.append((3 * seq[-1] + 2 * seq[-2] + 5) & MASK)
        for name in ['recurrence', 'recurrence-jump']: check(name, [n, a, b], seq[n])
for n in [44, 100, 1000]:
    a, b = 2, 3
    for _ in range(n): a, b = b, (3 * b + 2 * a + 5) & MASK
    check('recurrence-jump', [n, 2, 3], a)
for n in [0, 1, 2, 11, 100]:
    for a, b, seed in [(3, 5, 7), (0, MASK, MASK), (MASK, MASK, MASK),
                       tuple(rng.getrandbits(64) for _ in range(3))]:
        expected = seed
        for _ in range(n): expected = (a * expected + b) & MASK
        check('iterate', [n, a, b, seed], expected)
for n in [0, 1, 2, 3, 10, 99, 256, 4096]:
    check('string-length', [n], len(('end' + ''.join('x' if k % 2 else '😀' for k in range(1, n + 1))).encode('utf-16-le')) // 2)
for n in [0, 1, 2, 3, 4, 7, 19, 100, 1000]:
    rows = [(i, (i * 7919 - 97) & 0xffffffff) for i in range(n)]
    coefficient = functools.reduce(lambda a, row: a ^ row[1], rows, 7)
    result = 17
    for i, word in rows:
        result = ((word * coefficient - result) if i % 3 == 0 else
                  (word * 3 - result + (coefficient if i % 2 == 0 else -coefficient))) & 0xffffffff
    if result >= (1 << 31): result -= 1 << 32
    check('fibers', [n], result)

def generate(n, seed):
    values = []
    for _ in range(n):
        seed = (seed + 11400714819323198485) & MASK
        a = ((seed ^ (seed >> 30)) * 13787848793156543929) & MASK
        b = ((a ^ (a >> 27)) * 10723151780598845931) & MASK
        values.append(b ^ (b >> 31))
    return values

def check_values(args, values):
    # Closed-form sum independent of the C nested-pair loop.
    check('pair-energy', args, (2 * len(values) * sum(x*x for x in values) - 2 * sum(values)**2) & MASK)
    check('sorted-first', args, min(values, default=0))
    ordered = sorted(values)
    for rank in sorted({0, 1, len(values) // 2, max(0, len(values) - 1), len(values), MASK}):
        check('sorted-k', [rank, *args], ordered[rank] if rank < len(ordered) else 0)

for values in [[], [0], [MASK], [1, 2, 3], [9, 9, 9], [MASK, 0, MASK - 1],
               list(range(20)), list(range(20, -1, -1)), [rng.getrandbits(64) for _ in range(50)]]:
    check_values(values, values)
for n in [0, 1, 2, 15, 100]:
    for seed in [0, 42, MASK]: check_values(['--generate', n, seed], generate(n, seed))
    check_values(['--generate', n], generate(n, 42))
# Invalid syntax, signed and out-of-range integer rejection.
for name in ['ackermann', 'gibonacci', 'hanoi-fixed', 'iterate', 'recurrence',
             'recurrence-jump', 'pair-energy', 'sorted-first', 'string-length']:
    for bad in ['', '-1', '+1', ' 1', '1x', str(MASK + 1)]: check(name, [bad], code=2)
    check(name, ['1'] * 6, code=2) if name not in ['pair-energy', 'sorted-first'] else None
for name in ['pair-energy', 'sorted-first']:
    for args in [['--generate', 1048577], ['--generate', 'no'],
                 ['--generate', 1, '-1'], ['--generate', 0, 42, 9]]:
        check(name, args, code=2)
for name, args in [('hanoi-fixed', [64]), ('gibonacci', [2, 3, 63]),
                   ('string-length', [65537]), ('fibers', [-1]),
                   ('fibers', [10000001]), ('fibers', ['x']), ('fibers', [1, 2])]:
    check(name, args, code=2)
check('sorted-k', [], code=2)
for bad in ['', '-1', '+1', ' 1', '1x', str(MASK + 1)]:
    check('sorted-k', [bad], code=2)
    check('sorted-k', [0, bad], code=2)
for arguments in [['--generate', 1048577], ['--generate', 'no'],
                  ['--generate', 1, '-1'], ['--generate', 0, 42, 9]]:
    check('sorted-k', [0, *arguments], code=2)
for name in sorted(counts): print(f'{name}: {counts[name]} passed')
print(f'PASS: {sum(counts.values())} checks across {len(counts)} executables')
