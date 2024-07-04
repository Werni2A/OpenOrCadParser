#ifndef PRIMBASE_HPP
#define PRIMBASE_HPP

#include "Enums/Primitive.hpp"
#include "Record.hpp"

namespace OOCP
{
class PrimBase : public Base
{
public:
    PrimBase(StreamContext& aCtx)
        : Base{aCtx}
    {
    }

    virtual ~PrimBase()
    {
    }

    virtual Primitive getObjectType() const = 0;
};
} // namespace OOCP
#endif // PRIMBASE_HPP