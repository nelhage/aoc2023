#pragma once
#include "primitives.h"
#include "list.h"

template <typename T>
using as_digit = if_else<
    literal<T::value >= '0' && T::value <= '9'>,
    literal<T::value - '0'>,
    nil>;

template <typename Line, typename Find>
struct drop_until {};

template <typename...Elts, typename Find>
struct drop_until<list<Find, Elts...>, Find> {
    using type = list<Elts...>;
};

template <typename...Elts, typename El1, typename Find>
struct drop_until<list<El1, Elts...>, Find> {
    using type = typename drop_until<list<Elts...>, Find>::type;
};

template <typename Str, typename Accum=literal<0>>
struct parse_int {};

template <typename Accum>
struct parse_int<list<>, Accum> {
    using rest = list<>;
    using type = Accum;
};

template <typename El, typename... Elts, typename Accum>
struct parse_int<list<El, Elts...>, Accum> {
    using digit = typename as_digit<El>::type;

    struct IfDigit {
        using advance = parse_int<list<Elts...>, literal<10*Accum::value + digit::value>>;
        using rest = typename advance::rest;
        using type = typename advance::type;
    };
    struct NotDigit {
        using rest = list<El, Elts...>;
        using type = Accum;
    };
    using cond = typename if_else<
        typename is_nil<digit>::type,
        NotDigit,
        IfDigit
        >::type;

    using rest = typename cond::rest;
    using type = typename cond::type;
};

template <typename L, typename E>
struct split1 {};

template <typename... Elts, typename E>
struct split1<list<E, Elts...>, E> {
    using head = list<>;
    using tail = list<Elts...>;
};

template <typename E>
struct split1<list<>, E> {
    using head = list<>;
    using tail = list<>;
};

template <typename... Elts, typename H, typename E>
struct split1<list<H, Elts...>, E> {
    using recurse = split1<list<Elts...>, E>;

    using head = typename prepend<H, typename recurse::head>::type;
    using tail = typename recurse::tail;;
};
