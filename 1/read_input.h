#pragma once

#include "list.h"

template <auto... Elt>
struct read_input{
    using type = list<literal<char(Elt)>...>;
};

using problem = read_input<
    #include INPUT
>::type;
