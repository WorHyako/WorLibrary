#include "Network/UdpServer.hpp"

#include "Network/Utils/IoService.hpp"
#include "Wor/Log/Log.hpp"

#include <sstream>

using namespace Wor::Network;
using namespace boost;

UdpServer::~UdpServer() noexcept {
	stop();
}

void UdpServer::start(const Endpoint& endpoint) noexcept {
	stop();
	_socket = std::make_unique<Socket>(Utils::IoService::get());

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

	worInfo("Successful binding.");
	startReading();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void UdpServer::startReading() noexcept {
	static Endpoint remote_endpoint;
	asio::mutable_buffer m_buffer{_buffer.prepare(64)};
	_socket->async_receive_from(buffer(m_buffer),
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
										parseBuffer(remote_endpoint);
									}

									startReading();
								});
}

#pragma clang diagnostic pop

void UdpServer::stop() noexcept {
	if (!bound()) {
		return;
	}
	_socket->cancel();
	_socket->close();
	_socket.reset();
	worInfo("Stopped.");
}

void UdpServer::parseBuffer(const Endpoint& remote_endpoint) noexcept {
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
						  [&ss, &callbacks{_receive_callbacks}, &remote_endpoint](const std::string_view& message) {
							  ss << "\n\t\t" << message;
							  auto callback{
										  std::ranges::find_if(callbacks,
															   [&remote_endpoint](
														   const std::pair<Endpoint, Callback>& pair) {
																   return pair.first == remote_endpoint;
															   })
									  };
							  if (callback != std::end(callbacks) && callback->second) {
								  callback->second(message);
							  }
						  });
	worTrace(ss.str());
}

void UdpServer::sendTo(const Endpoint& endpoint, const std::string_view& message) noexcept {
	if (!bound()) {
		return;
	}
	_socket->async_send_to(asio::buffer(message),
						   endpoint,
						   [&endpoint, &message](const system::error_code& ec, std::size_t) {
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
								   worError("Sending message error: {}", ec.message());
								   return;
							   }
							   worInfo("Sending message success");
						   });
}

#pragma region Accessors/Mutators

bool UdpServer::bound() const noexcept {
	if (!_socket) {
		return false;
	}
	return _socket->is_open();
}

void UdpServer::receiveCallback(Endpoint remote_endpoint, Callback callback) noexcept {
	_receive_callbacks.emplace(std::move(remote_endpoint), std::move(callback));
}

UdpServer::Endpoint UdpServer::endpoint() const noexcept {
	return _socket->local_endpoint();
}

#pragma endregion Accessors/Mutators
