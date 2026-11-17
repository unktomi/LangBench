// Optimizer challenge: specialize a static bytecode program into native code.
// Run with Node.js 24 or later; no packages are required.
// All arithmetic is unsigned, wrapping modulo 2^64.
// Rounds and the initial state are runtime inputs; bytecode is static.
// Expected optimization: erase dispatch/instruction walking, NOT skip rounds.
//
// node static-vm.ts 0 42          -> 42
// node static-vm.ts 1 42          -> 5676762794293385329
// node static-vm.ts 2 42          -> 3874218066653976597
// node static-vm.ts 1000 42       -> 4584309983187257914
// node static-vm.ts 1000000 42    -> 1390335091984732934

const MASK = (1n << 64n) - 1n;
type Instruction = readonly [number, bigint];
// 0 = add, 1 = XOR with right-shifted state, 2 = multiply, 3 = XOR immediate.
const PROGRAM: readonly Instruction[] = [
  [0, 1234567890123456789n],
  [1, 17n],
  [2, 6364136223846793005n],
  [3, 1442695040888963407n],
  [1, 29n],
  [2, 2862933555777941757n],
  [1, 41n],
];

function interpret(code: readonly Instruction[], state: bigint): bigint {
  for (const [opcode, operand] of code) {
    switch (opcode) {
      case 0: state = (state + operand) & MASK; break;
      case 1: state = state ^ (state >> operand); break;
      case 2: state = (state * operand) & MASK; break;
      case 3: state = state ^ operand; break;
      default: throw new Error("unknown opcode");
    }
  }
  return state;
}

function benchmark(rounds: bigint, state: bigint): bigint {
  while (rounds > 0n) {
    state = interpret(PROGRAM, state);
    rounds -= 1n;
  }
  return state;
}

function u64(text: string): bigint {
  if (!/^[0-9]+$/.test(text)) throw new Error("expected an unsigned decimal integer");
  const value = BigInt(text);
  if (value > MASK) throw new Error("integer exceeds Uint64");
  return value;
}
try {
  const args = process.argv.slice(2);
  if (args.length > 2) throw new Error("too many arguments");
  console.log(benchmark(u64(args[0] ?? "1000000"), u64(args[1] ?? "42")).toString());
} catch (error) {
  console.error(String(error));
  console.error("usage: static-vm.ts [rounds [initial-state]]");
  process.exitCode = 2;
}
