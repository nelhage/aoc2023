#include <stdio.h>

#include <type_traits>

#include "list.h"
#include "read_input.h"

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

// template <typename C, typename L, typename R>

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

template <typename T>
struct solve {
    struct ScanFirst{};

    template <typename First, typename Last>
    struct ScanLast{};

    template <typename El,
              typename Accum,
              typename State>
    struct advance {
        using newline = literal<El::value == '\n'>;
        using digit = as_digit<El>::type;

        template <typename newline, typename digit, typename S>
        struct impl {
            using accum = Accum;
            using state = S;
        };

        template <int digit>
        struct impl<literal<false>, literal<digit>, ScanFirst> {
            using accum = Accum;
            using state = ScanLast<literal<digit>, literal<digit>>;
        };

        template <int digit, typename First, typename Rest>
        struct impl<literal<false>, literal<digit>, ScanLast<First, Rest>> {
            using accum = Accum;
            using state = ScanLast<First, literal<digit>>;
        };

        template <typename First, typename Rest>
        struct impl<literal<true>, nil, ScanLast<First, Rest>> {
            using accum = literal<Accum::value + First::value * 10 + Rest::value>;
            using state = ScanFirst;
        };

        using inner = impl<newline, digit, State>;
        using accum = inner::accum;
        using state = inner::state;
    };

    template <typename Rest, typename Accum, typename State>
    struct iter {};

    template <typename Fst, typename Rest, typename Accum, typename State>
    struct iter<pair<Fst, Rest>, Accum, State> {
        using out = advance<Fst, Accum, State>;
        using type = iter<Rest, typename out::accum, typename out::state>::type;
    };

    template <typename Accum>
    struct iter<nil, Accum, ScanFirst> {
        using type = Accum;
    };

    using type = iter<T, literal<0>, ScanFirst>::type;
    using split = split1<T, literal<'\n'>>;
    // using type = split::first;
    // static constexpr int debug = type();
};

template <typename T>
struct print {};

template <typename L, typename R>
struct print<pair<L, R>> {
    template <typename T>
    struct print_loop {};

    template <typename Ll, typename Rr>
    struct print_loop<pair<Ll, Rr>> {
        static void call() {
            printf("%d ", Ll::value);
            print_loop<Rr>::call();
        }
    };

    template <typename Ll>
    struct print_loop<pair<Ll, nil>> {
        static void call() {
            printf("%d", Ll::value);
        }
    };

    template <>
    struct print_loop<nil> {
        static void call() {}
    };

    static void call() {
        printf("[");
        print_loop<pair<L, R>>::call();
        printf("]\n");

    };
};

int main() {
    using answer = solve<problem>::type;
    printf("result=%d\n", answer::value);

    // using split = split1<problem, literal<'\n'>>;

    // print<split::first>::call();

    // using test = literal_list<1, 2, 3, 4>::type;

    // print<reverse<test>::type>::call();
    // print_list<answer>();
}
