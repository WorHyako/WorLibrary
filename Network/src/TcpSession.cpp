#include "Network/TcpSession.hpp"

#include <spdlog/spdlog.h>

using namespace Wor::Network;

using namespace boost::asio::ip;
using namespace boost;

TcpSession::TcpSession(asio::io_service& ioService) noexcept
	: _socket(ioService),
	  _isActive(false) {
}

void TcpSession::run() noexcept {
	if (!_isActive) {
		startReading();
		_isActive = true;
	}
}

void TcpSession::send(const std::string& message) noexcept {
	constexpr std::string_view endSymbol = "\r\n";
	_socket.async_write_some(asio::buffer(message + endSymbol.data()),
							 [self = shared_from_this(), message](const system::error_code& ec,
																  std::size_t bytesTransferred) {
								 std::stringstream ss;
								 ss << "TcpSession. Sending packet.\n Endpoint: "
										 << self->endpoint().address().to_string().c_str()
										 << ":"
										 << self->endpoint().port()
										 << "\nMessage: "
										 << message.c_str();
								 spdlog::info(ss.str());
								 if (ec || bytesTransferred == 0) {
									 spdlog::error("TcpSession. Error in sending packet");
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
			   [self = shared_from_this()](const system::error_code& ec, std::size_t bytesTransferred) {
				   if (!ec) {
					   self->startReading();
					   self->parseBuffer();
				   } else {
					   std::stringstream ss;
					   ss << "TcpSession. Error on reading packet.\nEndpoint: "
							   << self->endpoint().address().to_string().c_str()
							   << ":"
							   << self->endpoint().port()
							   << "\nError: "
							   << ec.what().c_str();
					   spdlog::error(ss.str());
					   self->close();
				   }
			   });
}

#pragma clang diagnostic pop

void TcpSession::parseBuffer() noexcept {
	const std::size_t size = std::distance(buffers_begin(_buffer.data()),
										   buffers_end(_buffer.data()));
	if (size == 0) {
		return;
	}
	std::string strBuffer = {
				buffers_begin(_buffer.data()),
				buffers_end(_buffer.data())
			};

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

	const tcp::endpoint endPoint = _socket.remote_endpoint();

	std::stringstream ss;
	ss << "TcpSession: Parse message from "
			<< endPoint.address().to_string()
			<< ":"
			<< endPoint.port()
			<< "\nMessages:\n";

	std::ranges::for_each(messages,
						  [&ss, &callback = _receiveCallback](const std::string& message) {
							  ss << message.c_str() << "\n";
							  if (callback) {
								  callback(message);
							  }
						  });
	spdlog::info(ss.str());
}

void TcpSession::close() noexcept {
	if (!_isActive) {
		return;
	}
	std::stringstream ss;
	ss << "TcpSession. Connection was closed.\nEndpoint: "
			<< _socket.remote_endpoint().address().to_string().c_str()
			<< ":"
			<< _socket.remote_endpoint().port();
	spdlog::info(ss.str());
	_isActive = false;
	_socket.close();
	if (_closeCallback) {
		_closeCallback(shared_from_this());
	}
}

TcpSession::ptr TcpSession::create(asio::io_service& ioService) noexcept {
	return TcpSession::ptr(new TcpSession(ioService));
}

#pragma region Accessors/Mutators

tcp::socket& TcpSession::socket() noexcept {
	return _socket;
}

void TcpSession::name(std::string name) noexcept {
	_name = std::move(name);
}

const std::string& TcpSession::name() noexcept {
	return _name;
}

tcp::endpoint TcpSession::endpoint() const noexcept {
	return _socket.is_open()
			   ? _socket.remote_endpoint()
			   : tcp::endpoint();
}

void TcpSession::closeCallback(std::function<void(TcpSession::ptr)> callback) noexcept {
	_closeCallback = std::move(callback);
}

void TcpSession::receiveCallback(std::function<void(const std::string& message)> callback) noexcept {
	_receiveCallback = std::move(callback);
}

#pragma endregion Accessors/Mutators
