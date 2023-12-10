#include <stdio.h>

#include "primitives.h"
#include "list.h"
#include "read_input.h"
#include "parsing.h"

#include <type_traits>
using std::is_same;

struct test_parse_int {
    using test = decltype("101x"_str);
    using got = parse_int<test>;

    static_assert(is_same<got::type, literal<101> >::value);
    static_assert(is_same<got::rest, typename value_list<'x'>::type>::value);
};

template <typename Color, typename Value>
struct is_one_legal {};

using blue = decltype("blue"_str);
using red = decltype("red"_str);
using green = decltype("green"_str);

template <typename Value>
struct is_one_legal<blue, Value> {
    using type = literal<Value::value <= 14>;
};

template <typename Value>
struct is_one_legal<green, Value> {
    using type = literal<Value::value <= 13>;
};

template <typename Value>
struct is_one_legal<red, Value> {
    using type = literal<Value::value <= 12>;
};

static_assert(is_same<
              is_one_legal<green, literal<10>>::type,
              literal<true>
              >::value);

static_assert(is_same<
              is_one_legal<green, literal<20>>::type,
              literal<false>
              >::value);

static_assert(is_same<
              is_one_legal<red, literal<20>>::type,
              literal<false>
              >::value);


template <typename Draw>
struct good_draw {
    using split = split1<typename tail<Draw>::type,
        literal<' '>>;

    using count = typename parse_int<typename split::head>::type;
    using type = typename is_one_legal<typename split::tail, count>::type;
};

template <typename Game>
struct is_legal_game {
    using type = typename fold_lines<
        typename All<good_draw>::Fn,
        typename All<good_draw>::initial,
        Game,
        literal<','>
        >::type;
};

static_assert(is_same<
              typename is_legal_game<decltype(" 3 blue, 4 red"_str)>::type,
              literal<true>
              >::value);

static_assert(is_same<
              typename is_legal_game<decltype(" 8 green, 6 blue, 20 red"_str)>::type,
              literal<false>
              >::value);

template <typename Line>
struct parse_line {
    using rest = typename drop_until<Line, literal<' '>>::type;

    using parseno = parse_int<rest>;
    using gameno = typename parseno::type;

    using is_legal = typename fold_lines<
        typename All<is_legal_game>::Fn,
        typename All<is_legal_game>::initial,
        typename tail<typename parseno::rest>::type,
        literal<';'>
        >::type;
};

struct test_line {
    using test = decltype("Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red"_str);
    using parsed = parse_line<test>;
    static_assert(is_same<
                  typename parsed::gameno,
                  literal<3>
                  >::value);
    static_assert(is_same<
                  typename parsed::is_legal,
                  literal<false>
                  >::value);

    using test2 = decltype("Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"_str);
    using parse2 = parse_line<test2>;
    static_assert(is_same<
                  typename parse2::gameno,
                  literal<1>
                  >::value);
    static_assert(is_same<
                  typename parse2::is_legal,
                  literal<true>
                  >::value);

    using test3 = decltype("Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red"_str);
    using parse3 = parse_line<test3>;
    static_assert(is_same<
                  typename parse3::gameno,
                  literal<4>
                  >::value);
    static_assert(is_same<
                  typename parse3::is_legal,
                  literal<false>
                  >::value);

};

template<typename State, typename Line>
struct Fn {
    using lineval = parse_line<Line>;

    using type = literal<
        if_else<typename lineval::is_legal,
                typename lineval::gameno,
                literal<0>
                >::type::value
        + State::value
        >;
};


using answer = fold_lines<Fn, literal<0>, problem>::type;

int main() {
    printf("%d\n", answer::value);
}
