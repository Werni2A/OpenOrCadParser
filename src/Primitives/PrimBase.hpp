#ifndef PRIMBASE_HPP
#define PRIMBASE_HPP


#include "Enums/Primitive.hpp"
#include "Record.hpp"


class PrimBase : public Base
{
public:
    PrimBase(StreamContext& aCtx) : Base{aCtx}
    { }

    virtual Primitive getObjectType() const = 0;
};


#endif // PRIMBASE_HPP