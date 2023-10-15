#include "Network/TcpSocket.hpp"

#include "Network/Utils/EndPointChecker.hpp"

using namespace Wor::Network;

namespace {

    /**
     * Boost socket context
     */
    boost::asio::io_context context;
}

TcpSocket::TcpSocket() noexcept
        : _socket(::context),
          _status(SocketStatus::ZeroCheck) {
}

TcpSocket::~TcpSocket() noexcept {
    if (_status == SocketStatus::Connected) {
        CloseSocket();
        _status = SocketStatus::ZeroCheck;
    }
}

void TcpSocket::CleanErrors() noexcept {
    _errors.clear();
}

std::size_t TcpSocket::Send(const std::string &message_) noexcept {
    std::size_t bytesSent = -1;
    if (_status != SocketStatus::Connected) {
        return bytesSent;
    }
    const auto buf = boost::asio::buffer(message_, (std::size_t) message_.size());
    bytesSent = _socket.send(buf);
    return bytesSent;
}

bool TcpSocket::TryToConnect() noexcept {
    _status = SocketStatus::Unreachable;
    auto destinationEndPoint = AsioTcpEndPoint(
            boost::asio::ip::address::from_string(_destinationEndPoint.address.c_str()),
            _destinationEndPoint.port);
    auto asyncConnect = std::async(std::launch::async,
                          [this, &destinationEndPoint]() {
                              _socket.connect(destinationEndPoint);
                          });
    try {
        auto timeout = asyncConnect.wait_for(std::chrono::milliseconds(100));
        if (timeout == std::future_status::timeout) {
            _socket.cancel();
            _socket.close();
            return false;
        }
    } catch (std::exception &e) {
        /// Temporary. Just for debug in development
        std::string errorMessage = e.what();
        return false;
    }
    _status = SocketStatus::Connected;
    return true;
}

void TcpSocket::CloseConnection() noexcept {
    CloseSocket();
}

bool TcpSocket::CheckEndPoint(const std::string &address) noexcept {
    return Wor::Network::Utils::checkEndPoint(address);
}

void TcpSocket::CloseSocket() noexcept {
    if (_socket.is_open()) {
        _socket.cancel();
        _socket.close();
    }
    _status = SocketStatus::Reachable;
}

#pragma region Accessors

const EndPoint &TcpSocket::DestinationEndPoint() const noexcept {
    return _destinationEndPoint;
}

const EndPoint &TcpSocket::SourceEndPoint() const noexcept {
    return _sourceEndPoint;
}

std::string TcpSocket::GetLastError() const noexcept {
    return _errors.back().message();
}

const std::vector<TcpSocket::BoostErrorCode> &TcpSocket::GetErrors() const noexcept {
    return _errors;
}

SocketStatus TcpSocket::Status() noexcept {
    return _status;
}

#pragma endregion Accessors

#pragma region Mutators

void TcpSocket::DestinationEndPoint(EndPoint endPoint_) noexcept {
    _destinationEndPoint = std::move(endPoint_);
}

void TcpSocket::SourceEndPoint(EndPoint endPoint_) noexcept {
    _sourceEndPoint = std::move(endPoint_);
}

#pragma endregion Mutators
