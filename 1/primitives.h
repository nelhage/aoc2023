#pragma once

template <auto v>
struct literal {
    constexpr static auto value = v;
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

template <typename Head, typename Tail>
struct pair {
    using head = Head;
    using tail = Tail;
};

template <typename If, typename Then, typename Else>
struct if_else {
    using type = Else;
};

template <typename Then, typename Else>
struct if_else<literal<true>, Then, Else> {
    using type = Then;
};

template <typename L, typename R>
struct or_else {
    using type = L;
};

template <typename R>
struct or_else<nil, R> {
    using type = R;
};
