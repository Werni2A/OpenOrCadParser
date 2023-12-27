#ifndef PRIMBASE_HPP
#define PRIMBASE_HPP


#include "Record.hpp"


class PrimBase : public Record
{
public:
    PrimBase(StreamContext& aCtx) : Record{aCtx}
    { }
};


#endif // PRIMBASE_HPP