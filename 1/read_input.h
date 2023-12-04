#pragma once

#include "list.h"

template <auto... Elts>
struct char_list {
    using type = typename literal_list<char(Elts)...>::type;
};

using problem = char_list<
    #include INPUT
>::type;
