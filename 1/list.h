#pragma once

template <auto v>
struct literal {
    constexpr static auto value = v;
};

template <typename Head, typename Tail>
struct pair {
    using head = Head;
    using tail = Tail;
};

struct nil;

template <typename T>
struct is_nil {
    using type = literal<false>;
};

template <>
struct is_nil<nil> {
    using type = literal<true>;
};

template <typename L, typename R>
struct or_else {
    using type = L;
};

template <typename R>
struct or_else<nil, R> {
    using type = R;
};

template <typename... Elts>
struct list {};

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


template <template<typename, typename> typename Fn,
    typename Init,
    typename List>
struct fold {};

template<template<typename, typename> typename Fn>
struct fold_helper {
    template <typename T>
    struct F {
        using type = T;

        template <typename R>
        auto operator<<(F<R>) {
            return F<typename Fn<T, R>::type>();
        };
    };
};

template <template<typename, typename> typename Fn,
          typename Init, typename... Elts>
struct fold<Fn, Init, list<Elts...>> {
    template <typename T>
    using F = fold_helper<Fn>::template F<T>;

    using type = decltype((F<Init>() << ... << F<Elts>()))::type;
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
