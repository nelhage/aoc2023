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
