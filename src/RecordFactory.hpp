#ifndef RECORDFACTORY_HPP
#define RECORDFACTORY_HPP

#include <memory>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "Primitives/PrimBase.hpp"
#include "Record.hpp"
#include "StreamContext.hpp"

namespace OOCP
{
class RecordFactory
{
public:
    static std::unique_ptr<Record> build(StreamContext& aCtx, Structure aStructure);
    static std::unique_ptr<PrimBase> build(StreamContext& aCtx, Primitive aPrimitive);
};
} // namespace OOCP
#endif // RECORDFACTORY_HPP