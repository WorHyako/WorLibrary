#include "Network/TcpSession.hpp"

#include "Wor/Log/Log.hpp"

using namespace Wor::Network;

using namespace boost;

TcpSession::TcpSession(IoContext& ctx) noexcept
	: _socket(ctx) {
}

void TcpSession::run() noexcept {
	if (!bound()) {
		startReading();
	}
}

void TcpSession::send(const std::string_view& message) noexcept {
	constexpr std::string_view end_symbol{"\r\n"};
	_socket.async_write_some(asio::buffer(std::string(message) + end_symbol.data()),
							 [self{shared_from_this()}, &message](const system::error_code& ec,
																  std::size_t bytes_transferred) {
								 worInfo("Sending packet to {}:{}\n\tPacket: {}.",
										 self->endpoint().address().to_string(),
										 self->endpoint().port(),
										 message);
								 if (ec) {
									 worError("Error sending message {}.", ec.message());
								 } else if (bytes_transferred < 1) {
									 worError("Error to send zero packet.");
								 } else {
									 worTrace("Success.");
								 }
							 });
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void TcpSession::startReading() noexcept {
	async_read(_socket,
			   _buffer,
			   asio::transfer_at_least(1),
			   [self{shared_from_this()}](const system::error_code& ec, std::size_t bytes_transferred) {
				   worInfo("Receive packet from {}:{}",
						   self->endpoint().address().to_string(),
						   self->endpoint().port());
				   if (ec) {
					   worError("Reading packet error: {}", ec.message());
					   self->close();
				   } else if (bytes_transferred < 1) {
					   worError("Received zero packet.");
				   } else {
					   self->parseBuffer();
				   }
				   self->startReading();
			   });
}

#pragma clang diagnostic pop

void TcpSession::parseBuffer() noexcept {
	const auto size{
				std::distance(buffers_begin(_buffer.data()),
							  buffers_end(_buffer.data()))
			};
	if (size == 0) {
		return;
	}
	std::string str_buffer{
				buffers_begin(_buffer.data()),
				buffers_end(_buffer.data())
			};

	_buffer.consume(size);

	std::vector<std::string> messages{};
	std::size_t begin{0};
	while (begin < size) {
		const std::size_t end{str_buffer.find('\0', begin)};
		if (end == static_cast<std::size_t>(-1)) {
			messages.emplace_back(str_buffer);
			break;
		}
		messages.emplace_back(str_buffer.substr(begin, end - begin));
		begin = end + 1;
	}

	std::stringstream ss;
	ss << "\tMessages:";

	std::ranges::for_each(messages,
						  [&ss, &callback{_receiveCallback}](const std::string_view& message) {
							  ss << "\n\t\t" << message;
							  if (callback) {
								  callback(message);
							  }
						  });
	worTrace(ss.str());
}

void TcpSession::close() noexcept {
	if (!bound()) {
		return;
	}
	worInfo("Session was closed.\nAlias: {}\nEndPoint: {}",
			alias(),
			_socket.remote_endpoint().address().to_string(),
			_socket.remote_endpoint().port());
	try {
		_socket.close();
	} catch (const system::system_error& e) {
		worError("Error in socket closing: {}. Descriptor was closed.", e.what());
	}
	if (_close_callback) {
		_close_callback(shared_from_this());
	}
}

TcpSession::ptr TcpSession::create(IoContext& ctx) noexcept {
	return TcpSession::ptr(new TcpSession(ctx));
}

#pragma region Accessors/Mutators

bool TcpSession::bound() const noexcept {
	return _socket.is_open();
}

TcpSession::Socket& TcpSession::socket() noexcept {
	return _socket;
}

void TcpSession::alias(const std::string_view& alias) noexcept {
	_alias = alias;
}

const std::string& TcpSession::alias() noexcept {
	return _alias;
}

TcpSession::Endpoint TcpSession::endpoint() const noexcept {
	return _socket.is_open()
			   ? _socket.remote_endpoint()
			   : Endpoint{};
}

void TcpSession::closeCallback(std::function<void(TcpSession::ptr)> callback) noexcept {
	_close_callback = std::move(callback);
}

void TcpSession::receiveCallback(Callback callback) noexcept {
	_receiveCallback = std::move(callback);
}

#pragma endregion Accessors/Mutators
