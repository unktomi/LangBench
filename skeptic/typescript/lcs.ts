// Optimizer challenge: discover sharing / dynamic programming in naive LCS.
// Run with Node.js 24 or later; no packages are required.
// Every input word is reduced modulo 4, including explicit CLI values.
// The first argument is the number of words in the left sequence.
//
// node lcs.ts 3 1 2 3 2 1 3                 -> 2
// node lcs.ts 4 0 1 2 3 0 2 3               -> 3
// node lcs.ts 10 --generate 20 42            -> 6
// node lcs.ts 200 --generate 400 42          -> 122
// The last case is an optimization challenge: naive recursion may not finish.

function lcs(
  left: readonly number[], right: readonly number[], i: number, j: number,
): number {
  if (i === left.length || j === right.length) return 0;
  if (left[i] === right[j]) return 1 + lcs(left, right, i + 1, j + 1);
  return Math.max(lcs(left, right, i + 1, j), lcs(left, right, i, j + 1));
}

// CLI/input support. The generator matches LangBench's BenchmarkInput.lang.
const MASK = (1n << 64n) - 1n;
function u64(text: string): bigint {
  if (!/^[0-9]+$/.test(text)) throw new Error("expected an unsigned decimal integer");
  const value = BigInt(text);
  if (value > MASK) throw new Error("integer exceeds Uint64");
  return value;
}
function mix(value: bigint): bigint {
  value = ((value ^ (value >> 30n)) * 13787848793156543929n) & MASK;
  value = ((value ^ (value >> 27n)) * 10723151780598845931n) & MASK;
  return value ^ (value >> 31n);
}
function input(args: string[]): bigint[] {
  if (args[0] !== "--generate") return args.map(u64);
  if (args.length > 3) throw new Error("too many generator arguments");
  const count = u64(args[1] ?? "400");
  if (count > 1048576n) throw new Error("generator count exceeds 1048576");
  let seed = u64(args[2] ?? "42");
  const values: bigint[] = [];
  for (let i = 0; i < Number(count); ++i) {
    seed = (seed + 11400714819323198485n) & MASK;
    values.push(mix(seed));
  }
  return values;
}
function main(args: string[]): void {
  if (args.length === 0) throw new Error("missing left-count");
  const requested = u64(args[0]);
  const values = input(args.slice(1)).map(x => Number(x % 4n));
  const split = Number(requested < BigInt(values.length) ? requested : BigInt(values.length));
  console.log(lcs(values.slice(0, split), values.slice(split), 0, 0));
}
try {
  main(process.argv.slice(2));
} catch (error) {
  console.error(String(error));
  console.error("usage: lcs.ts <left-count> [values...] | <left-count> --generate [total-count [seed]]");
  process.exitCode = 2;
}
