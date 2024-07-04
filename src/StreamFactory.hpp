#ifndef STREAMFACTORY_HPP
#define STREAMFACTORY_HPP

#include <filesystem>
#include <memory>

#include "ContainerContext.hpp"
#include "Stream.hpp"

namespace OOCP
{
class StreamFactory
{
public:
    static std::unique_ptr<Stream> build(ContainerContext& aCtx, const fs::path& aInputStream);
};
} // namespace OOCP

#endif // STREAMFACTORY_HPP