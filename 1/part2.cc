#include <stdio.h>

#include "list.h"
#include "read_input.h"

template <typename Head, typename Tail>
struct pair {
    using head = Head;
    using tail = Tail;
};


template <template<typename, typename> typename Fn, typename Delim>
struct fold_lines_f {
    template <typename In, typename Elt>
    struct F{
        using type = pair<
            typename append<typename In::head, Elt>::type,
            typename In::tail
            >;
    };

    template <typename A>
    struct F<A, Delim>{
        using type = pair<
            list<>,
            typename Fn<typename A::tail, typename A::head>::type
            >;
    };
};

template<template<typename, typename> typename Fn,
         typename Init,
         typename L,
         typename Delim = literal<'\n'>>
struct fold_lines {
    using type = fold<
        fold_lines_f<Fn, Delim>::template F,
        pair<list<>, Init>,
        L>::type::tail;
};

// Parsing state machine
struct S0 {};

// one
struct So {};
struct Son {};
// two
struct St {};
struct Stw {};
// three
struct Sth {};
struct Sthr {};
struct Sthre {};
// four
struct Sf {};
struct Sfo {};
struct Sfou {};
// five
struct Sfi {};
struct Sfiv {};
// six
struct Ss {};
struct Ssi {};
// seven
struct Sse {};
struct Ssev {};
struct Sseve {};
// eight
struct Se {};
struct Sei {};
struct Seig {};
struct Seigh {};
// nine
struct Sn {};
struct Sni {};
struct Snin {};

template <typename St, typename El>
struct next_state { using type = S0; };

// Initial letters
template<typename S> struct next_state<S, literal<'o'>> { using type = So; };
template<typename S> struct next_state<S, literal<'t'>> { using type = St; };
template<typename S> struct next_state<S, literal<'f'>> { using type = Sf; };
template<typename S> struct next_state<S, literal<'s'>> { using type = Ss; };
template<typename S> struct next_state<S, literal<'e'>> { using type = Se; };
template<typename S> struct next_state<S, literal<'n'>> { using type = Sn; };

// one
template<>           struct next_state<So,    literal<'n'>> { using type = Son; };
template<>           struct next_state<Son,   literal<'e'>> { using type = Se; };
template<>           struct next_state<Son,   literal<'i'>> { using type = Sni; };
// two
template<>           struct next_state<St,    literal<'w'>> { using type = Stw; };
// three
template<>           struct next_state<St,    literal<'h'>> { using type = Sth; };
template<>           struct next_state<Sth,   literal<'r'>> { using type = Sthr; };
template<>           struct next_state<Sthr,  literal<'e'>> { using type = Sthre; };
template<>           struct next_state<Sthre, literal<'i'>> { using type = Sei; };
// four
template<>           struct next_state<Sf,    literal<'o'>> { using type = Sfo; };
template<>           struct next_state<Sfo,   literal<'u'>> { using type = Sfou; };
template<>           struct next_state<Sfo,   literal<'n'>> { using type = Son; };
// five
template<>           struct next_state<Sf,    literal<'i'>> { using type = Sfi; };
template<>           struct next_state<Sfi,   literal<'v'>> { using type = Sfiv; };
// six
template<>           struct next_state<Ss,    literal<'i'>> { using type = Ssi; };
// seven
template<>           struct next_state<Ss,    literal<'e'>> { using type = Sse; };
template<>           struct next_state<Sse,   literal<'v'>> { using type = Ssev; };
template<>           struct next_state<Sse,   literal<'i'>> { using type = Sei; };
template<>           struct next_state<Ssev,  literal<'e'>> { using type = Sseve; };
template<>           struct next_state<Sseve, literal<'i'>> { using type = Sei; };
// eight
template<>           struct next_state<Se,    literal<'i'>> { using type = Sei; };
template<>           struct next_state<Sei,   literal<'g'>> { using type = Seig; };
template<>           struct next_state<Seig,  literal<'h'>> { using type = Seigh; };
// nine
template<>           struct next_state<Sn,    literal<'i'>> { using type = Sni; };
template<>           struct next_state<Sni,   literal<'n'>> { using type = Snin; };

// digits
template<typename S> struct next_state<S, literal<'0'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'1'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'2'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'3'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'4'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'5'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'6'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'7'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'8'>> { using type = S0; };
template<typename S> struct next_state<S, literal<'9'>> { using type = S0; };

// Producing digits
template <typename State, typename El>
struct matched_digit { using type = nil; };

template <> struct matched_digit<Son, literal<'e'>> { using type = literal<1>; };
template <> struct matched_digit<Stw, literal<'o'>> { using type = literal<2>; };
template <> struct matched_digit<Sthre, literal<'e'>> { using type = literal<3>; };
template <> struct matched_digit<Sfou, literal<'r'>> { using type = literal<4>; };
template <> struct matched_digit<Sfiv, literal<'e'>> { using type = literal<5>; };
template <> struct matched_digit<Ssi, literal<'x'>> { using type = literal<6>; };
template <> struct matched_digit<Sseve, literal<'n'>> { using type = literal<7>; };
template <> struct matched_digit<Seigh, literal<'t'>> { using type = literal<8>; };
template <> struct matched_digit<Snin, literal<'e'>> { using type = literal<9>; };

template<typename S> struct matched_digit<S, literal<'0'>> { using type = literal<0>; };
template<typename S> struct matched_digit<S, literal<'1'>> { using type = literal<1>; };
template<typename S> struct matched_digit<S, literal<'2'>> { using type = literal<2>; };
template<typename S> struct matched_digit<S, literal<'3'>> { using type = literal<3>; };
template<typename S> struct matched_digit<S, literal<'4'>> { using type = literal<4>; };
template<typename S> struct matched_digit<S, literal<'5'>> { using type = literal<5>; };
template<typename S> struct matched_digit<S, literal<'6'>> { using type = literal<6>; };
template<typename S> struct matched_digit<S, literal<'7'>> { using type = literal<7>; };
template<typename S> struct matched_digit<S, literal<'8'>> { using type = literal<8>; };
template<typename S> struct matched_digit<S, literal<'9'>> { using type = literal<9>; };


template <typename St, typename El>
struct TestF {
    using new_state = next_state<typename St::head, El>::type;
    using digit = matched_digit<typename St::head, El>::type;

    using type = pair<new_state,
                      typename append<typename St::tail, digit>::type>;
};

using test = read_input<'e', 'i', 'g', 'h', 't', 'w', 'o', 'n', 'e', 'x', '1'>::type;
using got = fold<TestF, pair<S0, list<>>, test>::type;

#include <type_traits>
using std::is_same;

static_assert(is_same<got::head, S0>::value);
static_assert(is_same<
              got::tail,
              list<
              nil, nil, nil, nil, literal<8>,
              nil, literal<2>,
              nil, literal<1>,
              nil,
              literal<1>>
              >::value);

template <typename MatchState, typename First, typename Last>
struct LineState {
    using state = MatchState;
    using first = First;
    using last = Last;
};

template <typename L, typename R>
struct or_else {
    using type = L;
};

template <typename R>
struct or_else<nil, R> {
    using type = R;
};

template <typename State, typename El>
struct LineF {
    using new_state = next_state<typename State::state, El>::type;
    using digit = matched_digit<typename State::state, El>::type;

    using type = LineState<
        new_state,
        typename or_else<typename State::first, digit>::type,
        typename or_else<digit, typename State::last>::type
        >;
};

template<typename Line>
struct calibration {
    using acc = fold<
        LineF,
        LineState<S0, nil, nil>,
        Line
        >::type;

    using type = literal<acc::first::value*10 + acc::last::value>;
};

static_assert(is_same<
              typename calibration<test>::type,
              literal<81>
              >::value);

static_assert(is_same<
              typename calibration<typename read_input<
              's', 'e', 'v', 'e', 'n', '8', '2', '6', '8', '3'
              >::type>::type,
              literal<73>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              '0', '1'
              >::type>::type,
              literal<1>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              '1', '0'
              >::type>::type,
              literal<10>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'o', 'n', 'e'
              >::type>::type,
              literal<11>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              't', 'w', 'o'
              >::type>::type,
              literal<22>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              't', 'h', 'r', 'e', 'e'
              >::type>::type,
              literal<33>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'f', 'o', 'u', 'r'
              >::type>::type,
              literal<44>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'f', 'i', 'v', 'e'
              >::type>::type,
              literal<55>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              's', 'i', 'x'
              >::type>::type,
              literal<66>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              's', 'e', 'v', 'e', 'n'
              >::type>::type,
              literal<77>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              's', 'e', 'v', 'e', 'n', 'i', 'n', 'e'
              >::type>::type,
              literal<79>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'e', 'i', 'g', 'h', 't'
              >::type>::type,
              literal<88>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'e', 'i', 'g', 'h', 't', 'w', 'o'
              >::type>::type,
              literal<82>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'n', 'i', 'n', 'e'
              >::type>::type,
              literal<99>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              'f', 'o', 'n', 'e'
              >::type>::type,
              literal<11>>::value);

static_assert(is_same<
              typename calibration<typename read_input<
              '8'
              >::type>::type,
              literal<88>>::value);


template<typename State, typename Line>
struct Fn {
    using lineval = calibration<Line>::type;

    using type = literal<State::value + lineval::value>;
};

int main() {
    using answer = fold_lines<Fn, literal<0>, problem>::type;

    printf("%d\n", answer::value);
}
