#include <stdio.h>

#include "primitives.h"
#include "list.h"
#include "read_input.h"
#include "parsing.h"

#include <type_traits>

template <typename Red = literal<0>,
          typename Green = literal<0>,
          typename Blue = literal<0>>
struct Counts {
    using red = Red;
    using green = Green;
    using blue = Blue;
};

template <typename L, typename R>
struct max {
    using type = literal<(L::value > R::value ? L::value : R::value)>;
};

template <typename L, typename R>
struct min {
    using type = literal<(L::value < R::value ? L::value : R::value)>;
};

template <typename L, typename R>
struct MaxCounts {
    using type = Counts<
        typename max<typename L::red,   typename R::red>::type,
        typename max<typename L::green, typename R::green>::type,
        typename max<typename L::blue,  typename R::blue>::type
        >;
};

using blue = value_list<'b', 'l', 'u', 'e'>::type;
using red = value_list<'r', 'e', 'd'>::type;
using green = value_list<'g', 'r', 'e', 'e', 'n'>::type;

template <typename Counts, typename Color, typename Value>
struct update_color {};

template <typename In, typename Value>
struct update_color<In, red, Value> {
    using type = Counts<
        Value,
        typename In::green,
        typename In::blue
        >;
};

template <typename In, typename Value>
struct update_color<In, green, Value> {
    using type = Counts<
        typename In::red,
        Value,
        typename In::blue
        >;
};

template <typename In, typename Value>
struct update_color<In, blue, Value> {
    using type = Counts<
        typename In::red,
        typename In::green,
        Value
        >;
};


template <typename In, typename Draw>
struct UpdateCounts {
    using split = split1<typename tail<Draw>::type,
        literal<' '>>;
    using count = typename parse_int<typename split::head>::type;

    using type = typename update_color<In, typename split::tail, count>::type;
};

template <typename Game>
struct GameCounts {
    using type = typename fold_lines<
        UpdateCounts,
        Counts<>,
        Game,
        literal<','>
        >::type;
};

#include <type_traits>
using std::is_same;

struct GameCounts_test {
    template <typename Line, typename Expect>
    struct test_case {
        static_assert(is_same<
                      typename GameCounts<Line>::type,
                      Expect>::value);
        using type = nil;
    };

    using t1 = test_case<
        typename value_list<
            ' ', '3', ' ', 'b', 'l', 'u', 'e', ',', ' ', '4', ' ', 'r', 'e', 'd'
            >::type,
        Counts<literal<4>, literal<0>, literal<3>>
        >::type;

    using t2 = test_case<
        typename value_list<
           ' ', '8', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '6', ' ', 'b', 'l', 'u', 'e', ',', ' ', '2', '0', ' ', 'r', 'e', 'd'
            >::type,
        Counts<literal<20>, literal<8>, literal<6>>
        >::type;
};

template <typename Line>
struct line_power {
    using rest = typename drop_until<Line, literal<' '>>::type;

    using parseno = parse_int<rest>;
    using gameno = typename parseno::type;

    using max_counts = typename fold_lines<
        typename MapFold<GameCounts, MaxCounts>::Fn,
        Counts<>,
        typename tail<typename parseno::rest>::type,
        literal<';'>
        >::type;
    using type = literal<
        max_counts::red::value * max_counts::green::value * max_counts::blue::value
        >;
};

template<typename Accum, typename Line>
struct Fn {
    using type = literal<
        line_power<Line>::type::value + Accum::value
        >;
};


using answer = fold_lines<Fn, literal<0>, problem>::type;

int main() {
    printf("%d\n", answer::value);
}
