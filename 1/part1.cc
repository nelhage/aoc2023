#include <stdio.h>

#include "primitives.h"
#include "list.h"
#include "read_input.h"

template <typename T>
using as_digit = if_else<
    literal<T::value >= '0' && T::value <= '9'>,
    literal<T::value - '0'>,
    nil>;


template <typename Accum, typename FirstDigit, typename LastDigit>
struct State {
    using accum=Accum;
    using first=FirstDigit;
    using last=LastDigit;
};

using empty_state = State<literal<0>, nil, nil>;

template <typename In, typename Char>
struct Fn {
    using digit = as_digit<Char>::type;

    struct IfNewline {
        using type = State<
            literal<In::accum::value + 10 * In::first::value + In::last::value>,
            nil,
            nil>;
    };

    using type = if_else<
        literal<Char::value == '\n'>,
        IfNewline,
        if_else<
            typename is_nil<digit>::type,
            In,
            typename if_else<
                typename is_nil<typename In::first>::type,
                State<typename In::accum, digit, digit>,
                State<typename In::accum, typename In::first, digit>
                >::type
            >>::type::type;
};

template <typename T>
struct solve {
    using type = fold<Fn, empty_state, T>::type::accum;
};

int main() {
    using answer = solve<problem>::type;
    printf("%d\n", answer::value);
}
