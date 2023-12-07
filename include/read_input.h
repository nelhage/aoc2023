#pragma once

#include "list.h"

template <auto... Elt>
struct read_input{
    using type = list<literal<char(Elt)>...>;
};


#define _QUOTED(x) #x
#define QUOTED(x) _QUOTED(x)

#ifndef INPUT
#define INPUT /dev/null
#endif

using problem = read_input<
#include QUOTED(INPUT)
>::type;
