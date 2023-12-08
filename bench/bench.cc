#include "list.h"
#include "primitives.h"
#include "read_input.h"

#include <stdio.h>

template<typename Acc, typename Line>
struct SumFn {
    using val = parse_int<Line>::type;
    using type = literal<Acc::value + val::value>;
};

template<typename L>
struct sum_lines {
    using type = fold_lines<
        SumFn,
        literal<0>,
        L
        >::type;
};


using answer = sum_lines<problem>::type;

void main () {
    printf("%d\n", answer::value);
}
