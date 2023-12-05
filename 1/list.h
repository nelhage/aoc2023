#pragma once

template <typename T>
struct head;

template <typename T>
struct tail;

template <typename L, typename R>
struct pair {
    using left = L;
    using right = R;
};

template <typename L, typename R>
struct head<pair<L, R>> {
    using type = L;
};

template <typename L, typename R>
struct tail<pair<L, R>> {
    using type = R;
};

struct nil {};

template <typename... Elts>
struct list {
    // using type = pair<Head, typename list<Elts...>::type>;
    using type = nil;
};

template <typename Head, typename... Elts>
struct list<Head, Elts...> {
    using type = pair<Head, typename list<Elts...>::type>;
};

/* literals */
template <auto v>
struct literal {
    constexpr static auto value = v;
};

template <auto... Elts>
struct literal_list {
    using type = list<literal<Elts>...>::type;
};

/* reverse */
template <typename Lst, typename Accum=nil>
struct reverse_helper {};

template <typename L, typename R, typename Accum>
struct reverse_helper<pair<L, R>, Accum> {
    using type = typename reverse_helper<R, pair<L, Accum>>::type;
};

template <typename Accum>
struct reverse_helper<nil, Accum> {
    using type = Accum;
};

template <typename Lst>
struct reverse {
    using type = reverse_helper<Lst>::type;
};


template <typename Lst, typename Find, typename Accum>
struct split1_helper {};

template<typename Find, typename Accum>
struct split1_helper<nil, Find, Accum> {
    using first = reverse<Accum>::type;
    using rest = nil;
};

template<typename Find, typename Accum, typename Rest>
struct split1_helper<pair<Find, Rest>, Find, Accum> {
    using first = reverse<Accum>::type;
    using rest = Rest;
};

template<typename Find, typename Accum, typename Fst, typename Rest>
struct split1_helper<pair<Fst, Rest>, Find, Accum> {
    using recurse = split1_helper<
        Rest,
        Find,
        pair<Fst, Accum>
        >;

    using first = recurse::first;
    using rest = recurse::rest;
};


template<typename Lst, typename Elt>
using split1 = split1_helper<Lst, Elt, nil>;
