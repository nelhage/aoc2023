#pragma once

template <auto v>
struct literal {
    constexpr static auto value = v;
};

struct nil;

template <typename... Elts>
struct list {};


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
