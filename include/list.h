#pragma once

#include "primitives.h"

template <typename... Elts>
struct list {};

template <auto... Elts>
struct value_list {
    using type = list<literal<Elts>...>;
};


template <typename L, typename E>
struct append {};

template <typename... Elts, typename E>
struct append<list<Elts...>, E> {
    using type = list<Elts..., E>;
};

template <typename L, typename E>
struct prepend {};

template <typename E, typename... Elts>
struct prepend<E, list<Elts...>> {
    using type = list<E, Elts...>;
};

template <typename L>
struct head {};

template <typename H, typename... Elts>
struct head<list<H, Elts...>> {
    using type = H;
};

template <typename L>
struct tail {};

template <typename H, typename... Elts>
struct tail<list<H, Elts...>> {
    using type = list<Elts...>;
};

// helper for string literals
template <typename T, T... chars>
constexpr list<literal<chars>...> operator""_str() { return { }; }


// folds

// Thanks to Gašper Ažman and Joshua Bronson for this faster fold
// implementation.
namespace _f {
  template<template<typename, typename> typename Fn, typename T>
  struct F;

  template<template<typename, typename> typename Fn, typename T>
  extern F<Fn, T> Fv;

  template<template<typename, typename> typename Fn, typename T>
  auto R(F<Fn, T> const&) -> T;

  template <template <typename, typename> typename Fn, typename T, typename R>
  auto operator<<=(F<Fn, T> const&, F<Fn, R> const&) -> F<Fn, typename Fn<T, R>::type> const&;

  template <template<typename, typename> typename Fn,
      typename Init,
      typename List>
  struct fold {};
  template <template<typename, typename> typename Fn,
            typename Init, typename... Elts>
  struct fold<Fn, Init, list<Elts...>> {
      using type = decltype(R((Fv<Fn, Init> <<= ... <<= Fv<Fn, Elts>)));
  };
}

using _f::fold;

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
    using state = typename fold<
        fold_lines_f<Fn, Delim>::template F,
        pair<list<>, Init>,
        L>::type;
    template <typename Final, typename State>
    struct maybe_fold_final {
        using type = typename Fn<State, Final>::type;
    };

    template <typename State>
    struct maybe_fold_final<list<>, State> {
        using type = State;
    };

    using type = typename maybe_fold_final<
        typename state::head,
        typename state::tail>::type;
};

// Convenience helpers for folds

template <template<typename> typename Map>
struct Any {
    template <typename State, typename Arg>
    struct Fn {};

    template <typename Arg>
    struct Fn<literal<true>, Arg> {
        using type = literal<true>;
    };

    template <typename Arg>
    struct Fn<literal<false>, Arg> {
        using type = typename Map<Arg>::type;
    };

    using initial = literal<false>;
};

template <template<typename> typename Map>
struct All {
    template <typename State, typename Arg>
    struct Fn {};

    template <typename Arg>
    struct Fn<literal<false>, Arg> {
        using type = literal<false>;
    };

    template <typename Arg>
    struct Fn<literal<true>, Arg> {
        using type = typename Map<Arg>::type;
    };

    using initial = literal<true>;
};

template <template<typename> typename Map,
          template<typename, typename> typename F
          >
struct MapFold {
    template <typename State, typename Val>
    struct Fn {
        using type = typename F<State, typename Map<Val>::type>::type;
    };
};
