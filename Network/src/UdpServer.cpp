#include "Network/UdpServer.hpp"

#include "Network/Utils/IoService.hpp"
#include "Wor/Log/Log.hpp"

#include <sstream>

using namespace Wor::Network;
using namespace boost;
using namespace boost::asio::ip;

UdpServer::~UdpServer() noexcept {
	stop();
}

void UdpServer::start(const Endpoint& endpoint) noexcept {
	stop();
	auto& ioService = Utils::IoService::get();
	_socket = std::make_unique<Socket>(ioService, endpoint);

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
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void UdpServer::startReading() noexcept {
	static Endpoint remoteEndpoint;
	asio::mutable_buffer mBuffer{_buffer.prepare(64)};
	_socket->async_receive_from(buffer(mBuffer),
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
										parseBuffer(remoteEndpoint);
									}

									startReading();
								});
}

#pragma clang diagnostic pop

void UdpServer::stop() noexcept {
	if (!isRunning()) {
		return;
	}
	_socket->cancel();
	_socket->close();
	_socket.reset();
	worInfo("Stopped.");
}

void UdpServer::parseBuffer(const Endpoint& remoteEndpoint) noexcept {
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
						  [&ss, &callbacks = _receiveCallbacks, &remoteEndpoint](const std::string& message) {
							  ss << "\n\t\t" << message.c_str();
							  auto callback = std::ranges::find_if(callbacks,
																   [&remoteEndpoint](
															   const std::pair<Endpoint, Callback>& pair) {
																	   return pair.first == remoteEndpoint;
																   });
							  if (callback != std::end(callbacks) && callback->second) {
								  callback->second(message);
							  }
						  });
	worTrace(ss.str());
}

void UdpServer::sendTo(const Endpoint& endpoint, const std::string& message) noexcept {
	if (!isRunning()) {
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
									   << message.c_str();
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

void UdpServer::receiveCallback(Endpoint remoteEndpoint, Callback callback) noexcept {
	_receiveCallbacks.emplace(std::move(remoteEndpoint), std::move(callback));
}

UdpServer::Endpoint UdpServer::endpoint() const noexcept {
	return _socket->local_endpoint();
}

#pragma endregion Accessors/Mutators
