#include "../../langbench/c/benchmark-input.h"

enum opcode { ADD, XOR_SHIFT, MULTIPLY, XOR_IMMEDIATE };
struct instruction { enum opcode op; uint64_t operand; };

static const struct instruction program[] = {
    {ADD, UINT64_C(1234567890123456789)},
    {XOR_SHIFT, 17},
    {MULTIPLY, UINT64_C(6364136223846793005)},
    {XOR_IMMEDIATE, UINT64_C(1442695040888963407)},
    {XOR_SHIFT, 29},
    {MULTIPLY, UINT64_C(2862933555777941757)},
    {XOR_SHIFT, 41},
};

static uint64_t interpret(uint64_t state) {
    for (size_t i = 0; i < sizeof(program) / sizeof(program[0]); ++i) {
        const struct instruction instruction = program[i];
        switch (instruction.op) {
        case ADD:           state += instruction.operand; break;
        case XOR_SHIFT:     state ^= state >> instruction.operand; break;
        case MULTIPLY:      state *= instruction.operand; break;
        case XOR_IMMEDIATE: state ^= instruction.operand; break;
        }
    }
    return state;
}

int main(int argc, char **argv) {
    uint64_t rounds = 1000000, state = 42;
    if (argc > 3 || (argc > 1 && !bench_parse(argv[1], &rounds)) ||
        (argc > 2 && !bench_parse(argv[2], &state)))
        return bench_usage("usage: static-vm [rounds [initial-state]]");
    while (rounds-- != 0) state = interpret(state);
    printf("%" PRIu64 "\n", state);
    return 0;
}
