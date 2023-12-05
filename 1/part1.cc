#include <stdio.h>

template <auto v>
struct literal {
    constexpr static auto value = v;
};

struct nil;

template <typename T> struct as_digit {using type = nil; };
template <char v> struct as_digit<literal<v>> {
    using l = literal<v >= '0'>;
    using r = literal<v <= '9'>;

    template <typename l, typename r>
    struct impl {
        using type = nil;
    };

    template <>
    struct impl<literal<true>, literal<true>> {
        using type = literal<v - '0'>;
    };

    using type = impl<l, r>::type;
};

template <typename... Elts>
struct list {};

template <auto... Elts>
struct value_list {};



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

template <typename Accum, typename FirstDigit, typename LastDigit>
struct State {
    using accum=Accum;
    using first=FirstDigit;
    using last=LastDigit;
};

using empty_state = State<literal<0>, nil, nil>;

template <typename In, typename Char>
struct Fn {
    template <typename newline, typename digit, typename first>
    struct impl {
        using type = In;
    };

    template <int digit>
    struct impl<literal<false>, literal<digit>, nil> {
        using type = State<typename In::accum, literal<digit>, literal<digit>>;
    };

    template <int digit, int first>
    struct impl<literal<false>, literal<digit>, literal<first>> {
        using type = State<typename In::accum,
            literal<first>,
            literal<digit>>;
    };

    template <int first>
    struct impl<literal<true>, nil, literal<first>> {
        using type = State<
            literal<In::accum::value + 10 * In::first::value + In::last::value>,
            nil,
            nil>;
    };

    using type = impl<
        literal<Char::value == '\n'>,
        typename as_digit<Char>::type,
        typename In::first
        >::type;
};


template <typename T>
struct solve {
    using type = fold<Fn, empty_state, T>::type::accum;
};


template <auto... Elt>
struct read_input{
    using type = list<literal<char(Elt)>...>;
};

using problem = read_input<
    #include INPUT
>::type;


int main() {
    using answer = solve<problem>::type;
    printf("result=%d\n", answer::value);
}
