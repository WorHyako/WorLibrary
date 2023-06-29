#include "Network/TcpSocket.hpp"

#include "Network/utils/EndPointChecker.hpp"

using namespace wor::network;

TcpSocket::TcpSocket(boost::asio::io_context &context) noexcept
        : _socket(context) {
}

TcpSocket::~TcpSocket() noexcept {
    CloseConnection();
}

void TcpSocket::CleanErrors() noexcept {
    _errors.clear();
}

std::size_t TcpSocket::Send(const std::string &message_) noexcept {
    std::size_t bytesSent = -1;
    if(!_socket.is_open()){
        return bytesSent;
    }
    auto buf = boost::asio::buffer(message_, (std::size_t) message_.size());
    bytesSent = _socket.send(buf);
    return bytesSent;
}

bool TcpSocket::TryToConnect() noexcept {
    auto destinationEndPoint = AsioTcpEndPoint(
            boost::asio::ip::address::from_string(_destinationEndPoint.address.c_str()),
            _destinationEndPoint.port);
    auto asy = std::async(std::launch::async,
                          [this, &destinationEndPoint]() {
                              _socket.connect(destinationEndPoint);
                          });
    try {
        auto timeout = asy.wait_for(std::chrono::milliseconds(100));
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
    return true;
}

void TcpSocket::CloseConnection() noexcept {
    if (_socket.is_open()) {
        _socket.close();
    }
}

bool TcpSocket::CheckEndPoint(const std::string &address) noexcept {
    return wor::network::utils::checkEndPoint(address);
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

const std::vector<boost::system::error_code> &TcpSocket::GetErrors() const noexcept {
    return _errors;
}

#pragma endregion Accessors

#pragma region Mutators

EndPoint::Status TcpSocket::DestinationEndPoint(EndPoint endPoint_) noexcept {
    _destinationEndPoint = std::move(endPoint_);
    return _destinationEndPoint.status;
}

EndPoint::Status TcpSocket::SourceEndPoint(EndPoint endPoint_) noexcept {
    _sourceEndPoint = std::move(endPoint_);
    return _sourceEndPoint.status;
}

#pragma endregion Mutators