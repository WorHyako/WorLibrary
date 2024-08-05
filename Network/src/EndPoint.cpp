#include "EndPoint.hpp"

using namespace Wor::Network;

EndPoint::EndPoint(std::string address, std::uint32_t port) noexcept
        : _address(std::move(address)),
          _port(port),
          _status(Status::ZeroCheck) {
}
