#include "transport.hpp"

#include "stdio_transport.hpp"

namespace towercpp {

std::unique_ptr<Transport> Transport::create(const Type type) {
    switch (type) {
        case Type::STDIO:
            return std::make_unique<StdioTransport>();
        default:
            break;
    }

    return nullptr;
}

}  // namespace towercpp