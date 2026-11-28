/* CLI adaptation of tests/modules/haskell/fixtures/osprey/upstream/suites/osprey-compute-kernels/reference/cases/binarytrees/binarytrees.c.
 * Only main reads runtime parameters; the reference algorithm is retained. */
#include "Osprey.Arguments.h"
/* Allocate + traverse many short-lived binary trees — allocation/memory stress.
 * Faithful port of binarytrees.osp: build 1200 trees of depth 13, sum checks. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Tree {
    struct Tree *left;
    struct Tree *right;
} Tree;

static Tree *make(int64_t d) {
    Tree *t = (Tree *)malloc(sizeof(Tree));
    if (d == 0) {
        t->left = NULL;
        t->right = NULL;
    } else {
        t->left = make(d - 1);
        t->right = make(d - 1);
    }
    return t;
}

static int64_t check(const Tree *t) {
    if (t == NULL) {
        return 0;
    }
    return 1 + check(t->left) + check(t->right);
}

static void freetree(Tree *t) {
    if (t == NULL) {
        return;
    }
    freetree(t->left);
    freetree(t->right);
    free(t);
}

int main(int argc, char **argv) {
    int64_t parameters[2];
    if (!osprey_arguments(argc, argv, 2, parameters)) return 2;
    const int64_t repetitions = parameters[0];
    const int64_t depth = parameters[1];
    int64_t acc = 0;
    for (int64_t i = 0; i < repetitions; i++) {
        Tree *t = make(depth);
        acc += check(t);
        freetree(t);
    }
    printf("%lld\n", (long long)acc);
    return 0;
}
