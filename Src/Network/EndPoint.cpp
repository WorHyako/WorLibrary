#include "WorLibrary/Network/EndPoint.hpp"

using namespace Wor::Network;

EndPoint::EndPoint(std::string address_, std::uint32_t port_) noexcept
        : address(std::move(address_)),
          port(port_),
          status(Status::ZeroCheck) {
}
