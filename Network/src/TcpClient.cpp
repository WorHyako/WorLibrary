#include "Network/TcpClient.hpp"

#include <memory>

#include "Wor/Log/Log.hpp"

using namespace Wor::Network;

using namespace boost;

TcpClient::TcpClient(IoContext& ctx) noexcept
	: _socket(ctx) {
}

void TcpClient::start(const Endpoint& endpoint) noexcept {
	if (!bound()) {
		return;
	}
	_socket.async_connect(endpoint,
						  [this, &endpoint](const system::error_code& ec) {
							  worInfo("Connecting to: {}:{}",
									  endpoint.address().to_string(),
									  endpoint.port());
							  if (ec) {
								  worError("Error in connection: {}", ec.message());
								  return;
							  }
							  worInfo("Connected.");
							  startRead();
						  });
}

void TcpClient::stop() noexcept {
	if (!bound()) {
		return;
	}
	try {
		_socket.cancel();
	} catch (const system::system_error& e) {
		worError("Error in socket canceling: {}.", e.what());
	}

	try {
		_socket.close();
	} catch (const system::system_error& e) {
		worError("Error in socket closing: {}. Descriptor was closed.", e.what());
	}
}

void TcpClient::startRead() noexcept {
	async_read_until(_socket,
					 _buffer,
					 '\n',
					 [this](const system::error_code& ec, std::size_t bytes_transferred) {
						 worInfo("Receive packet from {}:{}",
								 remoteEndpoint().address().to_string(),
								 remoteEndpoint().port());
						 if (ec) {
							 worError("Reading packet error: {}", ec.message());
						 } else if (bytes_transferred < 1) {
							 worError("Received zero packet.");
						 } else {
							 parseBuffer();
						 }
						 startRead();
					 });
}

void TcpClient::parseBuffer() noexcept {
	const auto size{
				std::distance(buffers_begin(_buffer.data()),
							  buffers_end(_buffer.data()))
			};
	if (size == 0) {
		return;
	}
	const std::string str_buffer{
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
						  [&ss, &callback{_receive_callback}](const std::string_view& message) {
							  ss << "\n\t\t" << message;
							  if (callback) {
								  callback(message);
							  }
						  });
	worTrace(ss.str());
}

void TcpClient::send(const std::string_view& message) noexcept {
	if (!bound()) {
		return;
	}
	async_write(_socket,
				asio::buffer(message),
				[this, &message](const system::error_code& ec, std::size_t bytes_transferred) {
					worInfo("Sending packet to {}:{}\n\tPacket: {}.",
							remoteEndpoint().address().to_string(),
							remoteEndpoint().port(),
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

#pragma region Accessors/Mutators

void TcpClient::receiveCallback(Callback callback) noexcept {
	_receive_callback = std::move(callback);
}

TcpClient::Endpoint TcpClient::remoteEndpoint() const noexcept {
	if (!bound()) {
		return {};
	}
	return _socket.remote_endpoint();
}

bool TcpClient::bound() const noexcept {
	return _socket.is_open();
}

#pragma endregion Accessors/Mutators
