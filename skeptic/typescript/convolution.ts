// Optimizer challenge: replace schoolbook polynomial multiplication with a
// subquadratic algorithm, while preserving exact arithmetic modulo 2^64.
// Run with Node.js 24 or later; no packages are required.
// Coefficients are in ascending power order. First argument splits the input.
// Output is a nonlinear, order-sensitive digest, not the sum of coefficients.
// --coefficients prints the complete coefficient vector for correctness checks.
//
// node convolution.ts --coefficients 3 1 2 3 4 5 -> 4 13 22 15
// node convolution.ts 3 1 2 3 4 5                -> 1715120742217437917
// node convolution.ts 1024 --generate 2048 42     -> 6712990061350587505

const MASK = (1n << 64n) - 1n;

function convolution(left: readonly bigint[], right: readonly bigint[]): bigint[] {
  if (left.length === 0 || right.length === 0) return [];
  const out: bigint[] = new Array(left.length + right.length - 1).fill(0n);
  for (let i = 0; i < left.length; ++i) {
    for (let j = 0; j < right.length; ++j) {
      out[i + j] = (out[i + j] + left[i] * right[j]) & MASK;
    }
  }
  return out;
}

function mix(value: bigint): bigint {
  value = ((value ^ (value >> 30n)) * 13787848793156543929n) & MASK;
  value = ((value ^ (value >> 27n)) * 10723151780598845931n) & MASK;
  return value ^ (value >> 31n);
}
function digest(coefficients: readonly bigint[]): bigint {
  let state = 14695981039346656037n;
  for (const coefficient of coefficients) state = mix(state ^ coefficient);
  return state;
}

// CLI/input support. The generator matches LangBench's BenchmarkInput.lang.
function u64(text: string): bigint {
  if (!/^[0-9]+$/.test(text)) throw new Error("expected an unsigned decimal integer");
  const value = BigInt(text);
  if (value > MASK) throw new Error("integer exceeds Uint64");
  return value;
}
function input(args: string[]): bigint[] {
  if (args[0] !== "--generate") return args.map(u64);
  if (args.length > 3) throw new Error("too many generator arguments");
  const count = u64(args[1] ?? "2048");
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
  const showCoefficients = args[0] === "--coefficients";
  if (showCoefficients) args = args.slice(1);
  if (args.length === 0) throw new Error("missing left-count");
  const requested = u64(args[0]);
  const values = input(args.slice(1));
  const split = Number(requested < BigInt(values.length) ? requested : BigInt(values.length));
  const coefficients = convolution(values.slice(0, split), values.slice(split));
  console.log(showCoefficients ? coefficients.map(String).join(" ") : digest(coefficients).toString());
}
try {
  main(process.argv.slice(2));
} catch (error) {
  console.error(String(error));
  console.error("usage: convolution.ts [--coefficients] <left-count> [values...] | <left-count> --generate [total-count [seed]]");
  process.exitCode = 2;
}
