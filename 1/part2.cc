#include <stdio.h>

#include "list.h"
#include "read_input.h"

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

// Producing digits
template <typename State, typename El>
struct match_digit { using type = nil; };

template <> struct match_digit<Son,   literal<'e'>> { using type = literal<1>; };
template <> struct match_digit<Stw,   literal<'o'>> { using type = literal<2>; };
template <> struct match_digit<Sthre, literal<'e'>> { using type = literal<3>; };
template <> struct match_digit<Sfou,  literal<'r'>> { using type = literal<4>; };
template <> struct match_digit<Sfiv,  literal<'e'>> { using type = literal<5>; };
template <> struct match_digit<Ssi,   literal<'x'>> { using type = literal<6>; };
template <> struct match_digit<Sseve, literal<'n'>> { using type = literal<7>; };
template <> struct match_digit<Seigh, literal<'t'>> { using type = literal<8>; };
template <> struct match_digit<Snin,  literal<'e'>> { using type = literal<9>; };

template<typename S> struct match_digit<S, literal<'0'>> { using type = literal<0>; };
template<typename S> struct match_digit<S, literal<'1'>> { using type = literal<1>; };
template<typename S> struct match_digit<S, literal<'2'>> { using type = literal<2>; };
template<typename S> struct match_digit<S, literal<'3'>> { using type = literal<3>; };
template<typename S> struct match_digit<S, literal<'4'>> { using type = literal<4>; };
template<typename S> struct match_digit<S, literal<'5'>> { using type = literal<5>; };
template<typename S> struct match_digit<S, literal<'6'>> { using type = literal<6>; };
template<typename S> struct match_digit<S, literal<'7'>> { using type = literal<7>; };
template<typename S> struct match_digit<S, literal<'8'>> { using type = literal<8>; };
template<typename S> struct match_digit<S, literal<'9'>> { using type = literal<9>; };


// Minimal self-test
#include <type_traits>
using std::is_same;

template <typename St, typename El>
struct TestF {
    using new_state = typename next_state<typename St::head, El>::type;
    using digit = typename match_digit<typename St::head, El>::type;

    using type = pair<new_state,
                      typename append<typename St::tail, digit>::type>;
};

using got = fold<TestF, pair<S0, list<>>,
                 decltype("eightwonex1"_str)
                 >::type;

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

// Computing calibration values
template <typename MatchState, typename First, typename Last>
struct CalibrationState {
    using state = MatchState;
    using first = First;
    using last = Last;
};

template <typename State, typename El>
struct LineF {
    using new_state = typename next_state<typename State::state, El>::type;
    using digit = typename match_digit<typename State::state, El>::type;

    using type = CalibrationState<
        new_state,
        typename or_else<typename State::first, digit>::type,
        typename or_else<digit, typename State::last>::type
        >;
};

template<typename Line>
struct calibration {
    using acc = typename fold<
        LineF,
        CalibrationState<S0, nil, nil>,
        Line
        >::type;

    using type = literal<acc::first::value*10 + acc::last::value>;
};

static_assert(is_same<
              typename calibration<decltype("seven82683"_str)>::type,
              literal<73>>::value);

static_assert(is_same<
              typename calibration<decltype("01"_str)>::type,
              literal<1>>::value);

static_assert(is_same<
              typename calibration<decltype("10"_str)>::type,
              literal<10>>::value);

static_assert(is_same<
              typename calibration<decltype("one"_str)>::type,
              literal<11>>::value);

static_assert(is_same<
              typename calibration<decltype("two"_str)>::type,
              literal<22>>::value);

static_assert(is_same<
              typename calibration<decltype("three"_str)>::type,
              literal<33>>::value);

static_assert(is_same<
              typename calibration<decltype("four"_str)>::type,
              literal<44>>::value);

static_assert(is_same<
              typename calibration<decltype("five"_str)>::type,
              literal<55>>::value);

static_assert(is_same<
              typename calibration<decltype("six"_str)>::type,
              literal<66>>::value);

static_assert(is_same<
              typename calibration<decltype("seven"_str)>::type,
              literal<77>>::value);

static_assert(is_same<
              typename calibration<decltype("sevenine"_str)>::type,
              literal<79>>::value);

static_assert(is_same<
              typename calibration<decltype("eight"_str)>::type,
              literal<88>>::value);

static_assert(is_same<
              typename calibration<decltype("eightwo"_str)>::type,
              literal<82>>::value);

static_assert(is_same<
              typename calibration<decltype("nine"_str)>::type,
              literal<99>>::value);

static_assert(is_same<
              typename calibration<decltype("fone"_str)>::type,
              literal<11>>::value);

static_assert(is_same<
              typename calibration<decltype("8"_str)>::type,
              literal<88>>::value);


template<typename State, typename Line>
struct Fn {
    using lineval = typename calibration<Line>::type;

    using type = literal<State::value + lineval::value>;
};

int main() {
    using answer = fold_lines<Fn, literal<0>, problem>::type;

    printf("%d\n", answer::value);
}
