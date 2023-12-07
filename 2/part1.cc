#include <stdio.h>

#include "primitives.h"
#include "list.h"
#include "read_input.h"
#include "parsing.h"

#include <type_traits>
using std::is_same;

struct test_parse_int {
    using test = typename value_list<'1', '0', '1', 'x'>::type;
    using got = parse_int<test>;

    static_assert(is_same<got::type, literal<101> >::value);
    static_assert(is_same<got::rest, typename value_list<'x'>::type>::value);
};

template <typename Color, typename Value>
struct is_one_legal {};

using blue = value_list<'b', 'l', 'u', 'e'>::type;
using red = value_list<'r', 'e', 'd'>::type;
using green = value_list<'g', 'r', 'e', 'e', 'n'>::type;

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

    using count = parse_int<typename split::head>::type;
    using type = is_one_legal<typename split::tail, count>::type;
};

template <typename Game>
struct is_legal_game {
    using type = fold_lines<
        typename All<good_draw>::Fn,
        typename All<good_draw>::initial,
        Game,
        literal<','>
        >::type;
};

static_assert(is_same<
              typename is_legal_game<
              typename value_list<' ', '3', ' ', 'b', 'l', 'u', 'e', ',', ' ', '4', ' ', 'r', 'e', 'd'>::type
              >::type,
              literal<true>
              >::value);

static_assert(is_same<
              typename is_legal_game<
                typename value_list<
                  ' ', '8', ' ', 'g', 'r', 'e', 'e', 'n', ',',
                  ' ', '6', ' ', 'b', 'l', 'u', 'e', ',',
                  ' ', '2', '0', ' ', 'r', 'e', 'd'
                >::type
              >::type,
              literal<false>
              >::value);

template <typename Line>
struct parse_line {
    using rest = drop_until<Line, literal<' '>>::type;

    using parseno = parse_int<rest>;
    using gameno = parseno::type;

    using is_legal = fold_lines<
        typename All<is_legal_game>::Fn,
        typename All<is_legal_game>::initial,
        typename tail<typename parseno::rest>::type,
        literal<';'>
        >::type;
};

struct test_line {
    using test = value_list<
        'G', 'a', 'm', 'e', ' ', '3', ':', ' ', '8', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '6', ' ', 'b', 'l', 'u', 'e', ',', ' ', '2', '0', ' ', 'r', 'e', 'd', ';', ' ', '5', ' ', 'b', 'l', 'u', 'e', ',', ' ', '4', ' ', 'r', 'e', 'd', ',', ' ', '1', '3', ' ', 'g', 'r', 'e', 'e', 'n', ';', ' ', '5', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '1', ' ', 'r', 'e', 'd'
        >::type;
    using parsed = parse_line<test>;
    static_assert(is_same<
                  typename parsed::gameno,
                  literal<3>
                  >::value);
    static_assert(is_same<
                  typename parsed::is_legal,
                  literal<false>
                  >::value);

    using test2 = value_list<
        'G', 'a', 'm', 'e', ' ', '1', ':', ' ', '3', ' ', 'b', 'l', 'u', 'e', ',', ' ', '4', ' ', 'r', 'e', 'd', ';', ' ', '1', ' ', 'r', 'e', 'd', ',', ' ', '2', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '6', ' ', 'b', 'l', 'u', 'e', ';', ' ', '2', ' ', 'g', 'r', 'e', 'e', 'n'
        >::type;
    using parse2 = parse_line<test2>;
    static_assert(is_same<
                  typename parse2::gameno,
                  literal<1>
                  >::value);
    static_assert(is_same<
                  typename parse2::is_legal,
                  literal<true>
                  >::value);

    using test3 = value_list<
        'G', 'a', 'm', 'e', ' ', '4', ':', ' ', '1', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '3', ' ', 'r', 'e', 'd', ',', ' ', '6', ' ', 'b', 'l', 'u', 'e', ';', ' ', '3', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '6', ' ', 'r', 'e', 'd', ';', ' ', '3', ' ', 'g', 'r', 'e', 'e', 'n', ',', ' ', '1', '5', ' ', 'b', 'l', 'u', 'e', ',', ' ', '1', '4', ' ', 'r', 'e', 'd'
        >::type;
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
