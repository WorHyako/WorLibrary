#include "Network/UdpClient.hpp"

#include "Network/Utils/IoService.hpp"

#include "Wor/Log/Log.hpp"

using namespace Wor::Network;
using namespace boost;

UdpClient::~UdpClient() noexcept {
	stop();
}

void UdpClient::start(const Endpoint& endpoint) noexcept {
	stop();
	auto& ctx{Utils::IoService::get()};
	_socket = std::make_unique<Socket>(ctx);

	worInfo("Binding to {}:{}", endpoint.address().to_string(), endpoint.port());

	system::error_code ec;
	std::ignore = _socket->open(endpoint.protocol(), ec);
	if (ec) {
		worError("Open error: {}", ec.message());
		return;
	}
	std::ignore = _socket->bind(endpoint, ec);
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

void UdpClient::send(const std::string_view& message) noexcept {
	if (!bound()) {
		return;
	}
	_socket->async_send_to(asio::buffer(message),
						   _remote_endpoint,
						   [&message, &endpoint{_remote_endpoint}](const system::error_code& ec, std::size_t) {
							   std::stringstream ss;
							   ss << "Sending message."
									   << "\n\tEndpoint: "
									   << endpoint.address().to_string().c_str()
									   << ":"
									   << endpoint.port()
									   << "\n\tMessage: "
									   << message;
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
	static Endpoint remote_endpoint;
	asio::mutable_buffer buffer{_buffer.prepare(64)};
	_socket->async_receive_from(buffer,
								remote_endpoint,
								[this](const system::error_code& ec, std::size_t bytes_transferred) {
									worInfo("Message was received from {}:{}",
											remote_endpoint.address().to_string(),
											remote_endpoint.port());
									if (ec) {
										worError("Error: {}", ec.message());
									} else if (bytes_transferred < 1) {
										worError("Zero bytes packet.");
									} else {
										_buffer.commit(bytes_transferred);
										parseBuffer();
									}

									startRead();
								});
}

void UdpClient::parseBuffer() noexcept {
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
						  [&ss, &callback{_read_callback}](const std::string_view& message) {
							  ss << "\n\t\t" << message;
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

void UdpClient::remoteEndpoint(Endpoint remote_endpoint) noexcept {
	_remote_endpoint = std::move(remote_endpoint);
}

void UdpClient::readCallback(Callback callback) noexcept {
	_read_callback = std::move(callback);
}

#pragma endregion Accessors/Mutators
