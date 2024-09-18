#include "Network/UdpClient.hpp"

#include "Network/Utils/IoService.hpp"

#include "Wor/Log/Log.hpp"

using namespace Wor::Network;
using namespace boost;
using namespace boost::asio::ip;

UdpClient::~UdpClient() noexcept {
	stop();
}

void UdpClient::start(const Endpoint& localEndpoint) noexcept {
	stop();
	auto& ctx = Utils::IoService::get();
	_socket = std::make_unique<Socket>(ctx);

	worInfo("Binding to {}:{}", localEndpoint.address().to_string(), localEndpoint.port());

	system::error_code ec;
	std::ignore = _socket->open(localEndpoint.protocol(), ec);
	if (ec) {
		worError("Open error: {}", ec.message());
		return;
	}
	std::ignore = _socket->bind(localEndpoint, ec);
	if (ec) {
		worError("Binding error: {}", ec.message());
		return;
	}

	worInfo("Successful opening local port");
	startRead();
}

void UdpClient::stop() noexcept {
	if (!bound()) {
		return;
	}
	_socket->cancel();
	_socket->close();
	_socket.reset();
	worInfo("Stopped.");
}

void UdpClient::send(const std::string& message) noexcept {
	if (!bound()) {
		return;
	}
	_socket->async_send_to(asio::buffer(message),
						   _remoteEndpoint,
						   [&message, &endpoint = _remoteEndpoint](const system::error_code& ec, std::size_t) {
							   std::stringstream ss;
							   ss << "Sending message."
									   << "\n\tEndpoint: "
									   << endpoint.address().to_string().c_str()
									   << ":"
									   << endpoint.port()
									   << "\n\tMessage: "
									   << message.c_str();
							   worTrace(ss.str());
							   if (ec) {
								   worError("Sending error:", ec.message());
								   return;
							   }
							   worInfo("Successful sending message: {}", message);
						   });
}

void UdpClient::startRead() noexcept {
	if (!bound()) {
		return;
	}
	static udp::endpoint remoteEndpoint;
	asio::mutable_buffer buffer{_buffer.prepare(64)};
	_socket->async_receive_from(buffer,
								remoteEndpoint,
								[this](const system::error_code& ec, std::size_t bytesTransferred) {
									worInfo("Message was received from {}:{}",
											remoteEndpoint.address().to_string(),
											remoteEndpoint.port());
									if (ec) {
										worError("Error: {}", ec.message());
									} else if (bytesTransferred < 1) {
										worError("Zero bytes packet.");
									} else {
										_buffer.commit(bytesTransferred);
										parseBuffer();
									}

									startRead();
								});
}

void UdpClient::parseBuffer() noexcept {
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

	std::stringstream ss;
	ss << "\tMessages:";
	std::ranges::for_each(messages,
						  [&ss, &callback = _readCallback](const std::string& message) {
							  ss << "\n\t\t" << message.c_str();
							  if (callback) {
								  callback(message);
							  }
						  });
	worTrace(ss.str());
}

#pragma region Accessors/Mutators

bool UdpClient::bound() const noexcept {
	if (!_socket) {
		return false;
	}
	return _socket->is_open();
}

UdpClient::Endpoint UdpClient::localEndpoint() const noexcept {
	return _socket->local_endpoint();
}

void UdpClient::remoteEndpoint(Endpoint remoteEndpoint) noexcept {
	_remoteEndpoint = std::move(remoteEndpoint);
}

void UdpClient::readCallback(Callback callback) noexcept {
	_readCallback = std::move(callback);
}

#pragma endregion Accessors/Mutators
