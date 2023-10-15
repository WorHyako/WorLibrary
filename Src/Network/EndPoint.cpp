#include "Network/EndPoint.hpp"

using namespace Wor::Network;

EndPoint::EndPoint(std::string address_, uint32_t port_) noexcept
        : address(std::move(address_)),
          port(port_),
          status(Status::ZeroCheck) {
}
