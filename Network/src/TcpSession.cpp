#include "TcpSession.hpp"

#include <cstdio>
#include <utility>

using namespace Wor::Network;

using namespace boost::asio::ip;
using namespace boost;

TcpSession::TcpSession(const std::shared_ptr<asio::io_service>& ioContext) noexcept
        : _socket(*ioContext),
          _isActive(false) {
}

void TcpSession::run() noexcept {
    if (!_isActive) {
        startReading();
        _isActive = true;
    }
}

void TcpSession::send(const std::string &message) noexcept {
    std::string endSymbol = "\r\n";
    _socket.async_write_some(asio::buffer(message + endSymbol),
                             [self = shared_from_this()]
                                     (const system::error_code &ec, std::size_t bytesTransferred) {
                                 std::printf("session send: \n\tbytes was transferred: %zu,\n\terror code: %i\n",
                                             bytesTransferred, ec.value());
                                 if (ec || bytesTransferred == 0) {
                                     self->close();
                                 }
                             });
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void TcpSession::startReading() noexcept {
    std::printf("startReading:\n");
    asio::async_read(_socket, _buffer,
                     asio::transfer_at_least(1),
                     [self = shared_from_this()]
                             (const system::error_code &ec, std::size_t bytesTransferred) {
                         if (!ec) {
                             std::printf("Received!\n\tbytes count: %zu\n",
                                         bytesTransferred);
                             tcp::endpoint endPoint = self->_socket.remote_endpoint();
                             self->startReading();
                             self->parseBuffer();
                         } else {
                             self->close();
                         }
                     });
}

#pragma clang diagnostic pop

void TcpSession::parseBuffer() noexcept {
    tcp::endpoint endPoint = _socket.remote_endpoint();

    std::printf("parseBuffer from %s:%i:\n", endPoint.address().to_string().c_str(), endPoint.port());
    std::string strBuffer = { asio::buffers_begin(_buffer.data()),
                              asio::buffers_end(_buffer.data()) };

    std::size_t size = std::distance(asio::buffers_begin(_buffer.data()),
                                     asio::buffers_end(_buffer.data()));
    _buffer.consume(size);

    std::vector<std::string> messages;
    std::size_t begin = 0;
    while (begin < size) {
        std::size_t end = strBuffer.find('\0', begin);
        if (end == std::size_t(-1)) {
            messages.emplace_back(strBuffer);
            break;
        }
        messages.emplace_back(strBuffer.substr(begin, end - begin));
        begin = end + 1;
    }

    std::for_each(std::begin(messages), std::end(messages), [](const std::string &message) {
        std::printf("\tmessage: %s\n", message.c_str());
    });
}

void TcpSession::close() {
    if (!_isActive) {
        return;
    }
    std::printf("Connection was closed.\n");
    _socket.close();
}

tcp::socket &TcpSession::socket() noexcept {
    return _socket;
}

TcpSession::ptr TcpSession::create(std::shared_ptr<asio::io_service> ioService) noexcept {
    return ptr(new TcpSession(std::move(ioService)));
}

#pragma region Accessors/Mutators

void TcpSession::setName(std::string name) noexcept {
    _name = std::move(name);
}

std::string TcpSession::getName() noexcept {
    return _name;
}

#pragma endregion Accessors/Mutators
