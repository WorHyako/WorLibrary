#include "TcpClient.hpp"

using namespace Wor::Network;

using namespace boost;
using namespace boost::asio::ip;

TcpClient::TcpClient(asio::io_service& ctx) noexcept
    : _socket(ctx),
      _acceptDeadline(ctx),
      _stopped(false) {
}

void TcpClient::start(const tcp::resolver::iterator& endPointIt) noexcept {
    // _acceptDeadline.expires_from_now(posix_time::seconds(60));

    _socket.async_connect(endPointIt->endpoint(), [this, endPointIt](const system::error_code& ec){
        if (!ec) {
            handleConnect(endPointIt);
            return;
        }
        std::printf("TcpClient::start::async:\n\tError in connection.\n\tError: %s\n", ec.what().c_str());
    });
}

void TcpClient::handleDeadline() noexcept {
    if(_stopped) {
        return;
    }
    if (_acceptDeadline.expires_at() <= asio::deadline_timer::traits_type::now()) {
        _socket.close();
        _acceptDeadline.expires_at(posix_time::pos_infin);
    }
    _acceptDeadline.async_wait([this](const system::error_code& ec) {
        if(ec) {
            std::printf("TcpClient::handleDeadline:\n\tError to:%s\n", ec.what().c_str());
        }
    });
}

void TcpClient::handleConnect(const tcp::resolver::iterator& endPoint) noexcept {
    if(_stopped) {
        return;
    }
    if(_socket.is_open()) {
        std::printf("TcpClient::handleConnect:\n\tConnected to:%s:%i\n",
            endPoint->endpoint().address().to_string().c_str(), endPoint->endpoint().port());
        startRead();
    }
}

void TcpClient::stop() noexcept {
    if (_stopped) {
        return;
    }
    _stopped = true;
    _socket.close();
    _acceptDeadline.cancel();
}

void TcpClient::startRead() noexcept {
    async_read_until(_socket, _readBuffer, '\n',
        [this](const system::error_code& ec, std::size_t) {
            if(ec) {
                std::printf("TcpClient::startRead::async_read:\n\tError: %s\n", ec.what().c_str());
                return;
            }
            handleRead();
        });
}

void TcpClient::handleRead() noexcept {
    if(_stopped) {
        return;
    }

    const std::size_t size = std::distance(buffers_begin(_readBuffer.data()),
                                        buffers_end(_readBuffer.data()));
    if(size == 0) {
        return;
    }
    std::string message = { buffers_begin(_readBuffer.data()),
                            buffers_end(_readBuffer.data()) };
    _readBuffer.consume(_readBuffer.size());

    if(message.empty()) {
        std::printf("TcpClient::handleRead:\n\tError: Empty string received\n");
    } else {
        std::printf("TcpClient::handleRead:\n\tMessage: %s\n", message.c_str());
    }
    if(_readCallback) {
        _readCallback(message);
    }
    startRead();
}

void TcpClient::send(const std::string& message) noexcept {
    if(_stopped) {
        return;
    }
    async_write(_socket, asio::buffer(message), [this](const system::error_code& ec, std::size_t bytes) {
        if(ec) {
            std::printf("TcpClient::startWrote:\n\tError: %s\n", ec.what().c_str());
            return;
        }
        handleWrite();
    });
}

void TcpClient::handleWrite() noexcept {
    if(_stopped) {
        return;
    }
}

#pragma region Accessors/Mutators

void TcpClient::readCallback(std::function<void(std::string)> callback) noexcept {
    _readCallback = std::move(callback);
}


bool TcpClient::isRunning() const noexcept {
    return !_stopped;
}

#pragma endregion Accessors/Mutators
