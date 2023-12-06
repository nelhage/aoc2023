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
struct NextState {
    using out = S0;
    using digit = nil;
};

// one
template<typename S>
struct NextState<S, literal<'o'>> {
    using out = So;
    using digit = nil;
};

template<>
struct NextState<So, literal<'n'>> {
    using out = Son;
    using digit = nil;
};

template<>
struct NextState<Son, literal<'e'>> {
    using out = Se;
    using digit = literal<1>;
};

template<>
struct NextState<Son, literal<'i'>> {
    using out = Sni;
    using digit = nil;
};

// two
template<typename S>
struct NextState<S, literal<'t'>> {
    using out = St;
    using digit = nil;
};

template<>
struct NextState<St, literal<'w'>> {
    using out = Stw;
    using digit = nil;
};

template<>
struct NextState<Stw, literal<'o'>> {
    using out = So;
    using digit = literal<2>;
};

// three
template<>
struct NextState<St, literal<'h'>> {
    using out = Sth;
    using digit = nil;
};

template<>
struct NextState<Sth, literal<'r'>> {
    using out = Sthr;
    using digit = nil;
};

template<>
struct NextState<Sthr, literal<'e'>> {
    using out = Sthre;
    using digit = nil;
};

template<>
struct NextState<Sthre, literal<'i'>> {
    using out = Sei;
    using digit = nil;
};

template<>
struct NextState<Sthre, literal<'e'>> {
    using out = Se;
    using digit = literal<3>;
};

// four
template<typename S>
struct NextState<S, literal<'f'>> {
    using out = Sf;
    using digit = nil;
};

template<>
struct NextState<Sf, literal<'o'>> {
    using out = Sfo;
    using digit = nil;
};

template<>
struct NextState<Sfo, literal<'u'>> {
    using out = Sfou;
    using digit = nil;
};

template<>
struct NextState<Sfo, literal<'n'>> {
    using out = Son;
    using digit = nil;
};

template<>
struct NextState<Sfou, literal<'r'>> {
    using out = S0;
    using digit = literal<4>;
};

// five
template<>
struct NextState<Sf, literal<'i'>> {
    using out = Sfi;
    using digit = nil;
};

template<>
struct NextState<Sfi, literal<'v'>> {
    using out = Sfiv;
    using digit = nil;
};

template<>
struct NextState<Sfiv, literal<'e'>> {
    using out = Se;
    using digit = literal<5>;
};

// six
template<typename S>
struct NextState<S, literal<'s'>> {
    using out = Ss;
    using digit = nil;
};

template<>
struct NextState<Ss, literal<'i'>> {
    using out = Ssi;
    using digit = nil;
};

template<>
struct NextState<Ssi, literal<'x'>> {
    using out = S0;
    using digit = literal<6>;
};

// seven
template<>
struct NextState<Ss, literal<'e'>> {
    using out = Sse;
    using digit = nil;
};

template<>
struct NextState<Sse, literal<'v'>> {
    using out = Ssev;
    using digit = nil;
};

template<>
struct NextState<Sse, literal<'i'>> {
    using out = Sei;
    using digit = nil;
};

template<>
struct NextState<Ssev, literal<'e'>> {
    using out = Sseve;
    using digit = nil;
};

template<>
struct NextState<Sseve, literal<'n'>> {
    using out = Sn;
    using digit = literal<7>;
};

template<>
struct NextState<Sseve, literal<'i'>> {
    using out = Sei;
    using digit = nil;
};

// eight
template<typename S>
struct NextState<S, literal<'e'>> {
    using out = Se;
    using digit = nil;
};

template<>
struct NextState<Se, literal<'i'>> {
    using out = Sei;
    using digit = nil;
};

template<>
struct NextState<Sei, literal<'g'>> {
    using out = Seig;
    using digit = nil;
};

template<>
struct NextState<Seig, literal<'h'>> {
    using out = Seigh;
    using digit = nil;
};

template<>
struct NextState<Seigh, literal<'t'>> {
    using out = St;
    using digit = literal<8>;
};

// nine
template<typename S>
struct NextState<S, literal<'n'>> {
    using out = Sn;
    using digit = nil;
};

template<>
struct NextState<Sn, literal<'i'>> {
    using out = Sni;
    using digit = nil;
};

template<>
struct NextState<Sni, literal<'n'>> {
    using out = Snin;
    using digit = nil;
};

template<>
struct NextState<Snin, literal<'e'>> {
    using out = Se;
    using digit = literal<9>;
};

// digits
template<typename S>
struct NextState<S, literal<'0'>> {
    using out = S0;
    using digit = literal<0>;
};
template<typename S>
struct NextState<S, literal<'1'>> {
    using out = S0;
    using digit = literal<1>;
};
template<typename S>
struct NextState<S, literal<'2'>> {
    using out = S0;
    using digit = literal<2>;
};
template<typename S>
struct NextState<S, literal<'3'>> {
    using out = S0;
    using digit = literal<3>;
};
template<typename S>
struct NextState<S, literal<'4'>> {
    using out = S0;
    using digit = literal<4>;
};
template<typename S>
struct NextState<S, literal<'5'>> {
    using out = S0;
    using digit = literal<5>;
};
template<typename S>
struct NextState<S, literal<'6'>> {
    using out = S0;
    using digit = literal<6>;
};
template<typename S>
struct NextState<S, literal<'7'>> {
    using out = S0;
    using digit = literal<7>;
};
template<typename S>
struct NextState<S, literal<'8'>> {
    using out = S0;
    using digit = literal<8>;
};
template<typename S>
struct NextState<S, literal<'9'>> {
    using out = S0;
    using digit = literal<9>;
};

template <typename St, typename El>
struct TestF {
    using next = NextState<typename St::head, El>;
    using type = pair<typename next::out,
                      typename append<typename St::tail, typename next::digit>::type>;
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

    using have_first = is_nil<first>::type;
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
    using step = NextState<typename State::state, El>;

    using type = LineState<
        typename step::out,
        typename or_else<typename State::first, typename step::digit>::type,
        typename or_else<typename step::digit, typename State::last>::type
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

    using type = literal<
        State::value + lineval::value
        >;
};

int main() {
    using answer = fold_lines<Fn, literal<0>, problem>::type;

    printf("%d\n", answer::value);
}
