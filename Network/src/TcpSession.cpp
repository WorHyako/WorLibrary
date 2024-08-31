#include "Network/TcpSession.hpp"

#include <cstdio>

using namespace Wor::Network;

using namespace boost::asio::ip;
using namespace boost;

TcpSession::TcpSession(asio::io_service &ioService) noexcept
	: _socket(ioService),
	  _isActive(false) {
}

void TcpSession::run() noexcept {
	if (!_isActive) {
		startReading();
		_isActive = true;
	}
}

void TcpSession::send(const std::string &message) noexcept {
	constexpr std::string_view endSymbol = "\r\n";
	_socket.async_write_some(asio::buffer(message + endSymbol.data()),
							 [self = shared_from_this(), message](const system::error_code &ec,
																  std::size_t bytesTransferred) {
								 std::printf(
										 "TcpSession::send:\n\tremote client: %s%i\n\tmessage:%s\n\tbytes count: %zu\n",
										 self->endpoint().address().to_string().c_str(),
										 self->endpoint().port(),
										 message.c_str(),
										 bytesTransferred);
								 if (ec || bytesTransferred == 0) {
									 // self->close();
								 }
							 });
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void TcpSession::startReading() noexcept {
	async_read(_socket,
			   _buffer,
			   asio::transfer_at_least(1),
			   [self = shared_from_this()](const system::error_code &ec, std::size_t bytesTransferred) {
				   if (!ec) {
					   self->startReading();
					   self->parseBuffer();
				   } else {
					   std::printf("TcpSession::startReading::async_read:\n\tError: %s\n", ec.what().c_str());
					   self->close();
				   }
			   });
}

#pragma clang diagnostic pop

void TcpSession::parseBuffer() noexcept {
	const tcp::endpoint endPoint = _socket.remote_endpoint();

	std::printf("TcpSession::parseBuffer:\n\tsender: %s:%i",
				endPoint.address().to_string().c_str(),
				endPoint.port());
	const std::size_t size = std::distance(buffers_begin(_buffer.data()),
										   buffers_end(_buffer.data()));
	if (size == 0) {
		return;
	}
	std::string strBuffer = {buffers_begin(_buffer.data()),
							 buffers_end(_buffer.data())};

	_buffer.consume(size);

	std::vector<std::string> messages;
	std::size_t begin = 0;
	while (begin < size) {
		const std::size_t end = strBuffer.find('\0', begin);
		if (end == static_cast<std::size_t>(-1)) {
			messages.emplace_back(strBuffer);
			break;
		}
		messages.emplace_back(strBuffer.substr(begin, end - begin));
		begin = end + 1;
	}

	std::ranges::for_each(messages,
						  [](const std::string &message) {
							  std::printf("\n\tmessage: %s\n", message.c_str());
						  });
}

void TcpSession::close() {
	if (!_isActive) {
		return;
	}
	std::printf("TcpSession:\n\tConnection was closed.\n\tEndpoint: %s:%i\n",
				_socket.remote_endpoint().address().to_string().c_str(),
				_socket.remote_endpoint().port());
	_isActive = false;
	_socket.close();
	if (_closeCallback) {
		_closeCallback(shared_from_this());
	}
}

tcp::socket &TcpSession::socket() noexcept {
	return _socket;
}

TcpSession::ptr TcpSession::create(asio::io_service &ioService) noexcept {
	return ptr(new TcpSession(ioService));
}

#pragma region Accessors/Mutators

void TcpSession::name(std::string name) noexcept {
	_name = std::move(name);
}

std::string TcpSession::name() noexcept {
	return _name;
}

tcp::endpoint TcpSession::endpoint() const noexcept {
	return _socket.is_open() ? _socket.remote_endpoint() : boost::asio::ip::tcp::endpoint();
}

void TcpSession::closeCallback(std::function<void(TcpSession::ptr)> callback) noexcept {
	_closeCallback = std::move(callback);
}

#pragma endregion Accessors/Mutators
