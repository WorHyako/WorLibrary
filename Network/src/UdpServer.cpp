#include "Network/UdpServer.hpp"

#include "Network/Utils/IoService.hpp"
#include "Wor/Log/Log.hpp"

#include <spdlog/spdlog.h>

#include <sstream>

using namespace Wor::Network;
using namespace boost;
using namespace boost::asio::ip;

UdpServer::~UdpServer() noexcept {
	stop();
}

bool UdpServer::bindTo(const udp::endpoint& endpoint) noexcept {
	stop();
	auto& ioService = Utils::IoService::get();
	_socket = std::make_unique<udp::socket>(ioService, endpoint);
	std::stringstream ss;
	ss << "UdpServer. Start binding to: "
			<< endpoint.address().to_string().c_str()
			<< ":"
			<< endpoint.port();
	worInfo(ss.str());

	if (!_socket->is_open()) {
		worError("Udp server. Open port error.");
		return false;
	}

	// system::error_code ec;
	// std::ignore = _socket->bind(endpoint, ec);
	//
	// if (ec) {
	// 	spdlog::error("Udp server. Binding to local port error: {}", ec.message());
	// 	return false;
	// }
	//
	// std::ignore = _socket->open(endpoint.protocol(), ec);
	// if (ec) {
	// 	spdlog::error("Udp server. Open port error: {}", ec.message());
	// 	return false;
	// }
	worInfo("UdpServer. Successful binding.");
	return true;
}

void UdpServer::start() noexcept {
	if (!isRunning()) {
		return;
	}
	startReading();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void UdpServer::startReading() noexcept {
	static udp::endpoint remoteEndpoint;
	asio::mutable_buffer mBuffer = _buffer.prepare(1024);
	_socket->async_receive_from(buffer(mBuffer),
								remoteEndpoint,
								[this](const system::error_code& ec, std::size_t bytesTransferred) {
									if (ec) {
										std::stringstream ss;
										ss << "UdpServer. Error on reading packet.\nEndpoint: "
												<< endpoint().address().to_string().c_str()
												<< ":"
												<< endpoint().port()
												<< "\nError: "
												<< ec.what().c_str();
										worError(ss.str());
										startReading();
									} else {
										std::stringstream ss;
										ss << "UdpServer. Receiving packet.\nEndpoint: "
												<< remoteEndpoint.address().to_string().c_str()
												<< ":"
												<< remoteEndpoint.port();
										worTrace(ss.str());
										startReading();
										parseBuffer(remoteEndpoint);
									}
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
}

void UdpServer::parseBuffer(const udp::endpoint& remoteEndpoint) noexcept {
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
	ss << "UdpServer. Parsed message from "
			<< remoteEndpoint.address().to_string()
			<< ":"
			<< remoteEndpoint.port()
			<< "\nMessages:\n";

	std::ranges::for_each(messages,
						  [&ss, &callbacks = _receiveCallbacks, &remoteEndpoint](const std::string& message) {
							  ss << message.c_str() << "\n";
							  auto callback = std::ranges::find_if(callbacks,
																   [&remoteEndpoint](
															   const std::pair<udp::endpoint, std::function<void (
																				   const std::string&)>>& pair) {
																	   return pair.first == remoteEndpoint;
																   });
							  if (callback != std::end(callbacks) && callback->second) {
								  callback->second(message);
							  }
						  });
	worTrace(ss.str());
}

#pragma region Accessors/Mutators

bool UdpServer::isRunning() const noexcept {
	if (!_socket) {
		return false;
	}
	return _socket->is_open();
}

void UdpServer::receiveCallback(udp::endpoint remoteEndpoint,
								std::function<void(const std::string&)> callback) noexcept {
	_receiveCallbacks.emplace(std::move(remoteEndpoint), std::move(callback));
}

udp::endpoint UdpServer::endpoint() const noexcept {
	return _socket->local_endpoint();
}

#pragma endregion Accessors/Mutators
