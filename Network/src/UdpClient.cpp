#include "Network/UdpClient.hpp"

#include <Network/Utils/IoService.hpp>

#include "Wor/Log/Log.hpp"

#include <spdlog/spdlog.h>

using namespace Wor::Network;
using namespace boost;
using namespace boost::asio;

void UdpClient::start(const ip::udp::endpoint& localEndpoint) noexcept {
	auto& ctx = Utils::IoService::get();
	system::error_code ec;
	_socket = std::make_unique<ip::udp::socket>(ctx, localEndpoint);
	std::stringstream ss;
	ss << localEndpoint.address().to_string()
			<< ":"
			<< localEndpoint.port();
	if (!_socket->is_open()) {
		worError("Binding to local port error", ec.message());
		return;
	}
	//
	// ec = _socket.open(localEndpoint.protocol(), ec);
	// if (ec) {
	// 	Log::write({"Opening local port error", ec.message()}, Log::Type::Error);
	// 	return;
	// }

	worInfo("Successful opening local port");
}

void UdpClient::stop() noexcept {
	_socket->cancel();
	_socket->close();
}

void UdpClient::send(const std::string& message) noexcept {
	if (!isRunning()) {
		return;
	}
	_socket->async_send_to(buffer(message),
						   _remoteEndpoint,
						   [&message](const system::error_code& ec, std::size_t bytesTransferred) {
							   if (ec) {
								   worError("Sending error:", ec.message());
								   return;
							   }
							   worInfo(std::format("Successful sending message: {}", message));
						   });
}

void UdpClient::startRead() noexcept {
	if (!isRunning()) {
		return;
	}
	mutable_buffer buffer = _buffer.prepare(1024);
	_socket->async_receive(buffer,
						   [this](const system::error_code& ec, std::size_t bytesTransferred) {
							   if (ec) {
								   worError("Reading error:", ec.what());
								   return;
							   }

							   if (bytesTransferred < 1) {
								   worError("Received zero bytes:");
								   return;
							   }

							   parseBuffer();
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

	const ip::udp::endpoint endPoint = _socket->remote_endpoint();

	std::stringstream ss;
	ss << "TcpSession: Parse message from "
			<< endPoint.address().to_string()
			<< ":"
			<< endPoint.port()
			<< "\nMessages:\n";

	std::ranges::for_each(messages,
						  [&ss, &callback = _readCallback](const std::string& message) {
							  ss << message.c_str() << "\n";
							  if (callback) {
								  callback(message);
							  }
						  });
	worTrace(ss.str());
}

#pragma region Accessors/Mutators

bool UdpClient::isRunning() const noexcept {
	if (!_socket) {
		return false;
	}
	return _socket->is_open();
}

ip::udp::endpoint UdpClient::localEndpoint() const noexcept {
	return _socket->local_endpoint();
}

void UdpClient::remoteEndpoint(ip::udp::endpoint remoteEndpoint) noexcept {
	_remoteEndpoint = std::move(remoteEndpoint);
}

void UdpClient::readCallback(std::function<void(std::string)> callback) noexcept {
	_readCallback = std::move(callback);
}

#pragma endregion Accessors/Mutators
