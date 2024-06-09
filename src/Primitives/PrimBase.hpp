#ifndef PRIMBASE_HPP
#define PRIMBASE_HPP


#include "Record.hpp"


class PrimBase : public Base
{
public:
    PrimBase(StreamContext& aCtx) : Base{aCtx}
    { }
};


#endif // PRIMBASE_HPP