#include "Network/TcpClient.hpp"

#include <memory>

#include "spdlog/spdlog.h"

using namespace Wor::Network;

using namespace boost;
using namespace boost::asio::ip;

TcpClient::TcpClient(asio::io_service& ctx) noexcept
	: _socket(ctx),
	  _stopped(false) {
}

void TcpClient::start(const tcp::resolver::iterator& endPointIt) noexcept {
	_socket.async_connect(endPointIt->endpoint(),
						  [this, endPointIt](const system::error_code& ec) {
							  if (ec) {
								  std::stringstream ss;
								  ss << "TcpClient: Error in connection.\n"
										  << "Error:"
										  << ec.message();
								  spdlog::error(ss.str());
								  return;
							  }
							  handleConnect(endPointIt);
						  });
}

void TcpClient::handleConnect(const tcp::resolver::iterator& endPoint) noexcept {
	if (_stopped) {
		return;
	}
	if (_socket.is_open()) {
		std::stringstream ss;
		ss << "TcpClient: Connected to:"
				<< endPoint->endpoint().address().to_string().c_str()
				<< ":"
				<< endPoint->endpoint().port();
		spdlog::info(ss.str());
		startRead();
	}
}

void TcpClient::stop() noexcept {
	if (_stopped) {
		return;
	}
	_stopped = true;
	_socket.cancel();
	_socket.close();
}

void TcpClient::startRead() noexcept {
	async_read_until(_socket,
					 _readBuffer,
					 '\n',
					 [this](const system::error_code& ec, std::size_t) {
						 if (ec) {
							 std::stringstream ss;
							 ss << "TcpClient. Reading error:"
									 << ec.what().c_str();
							 spdlog::error(ss.str());
							 return;
						 }
						 handleRead();
					 });
}

void TcpClient::handleRead() noexcept {
	if (_stopped) {
		return;
	}

	const std::size_t size = std::distance(buffers_begin(_readBuffer.data()),
										   buffers_end(_readBuffer.data()));
	if (size == 0) {
		return;
	}
	const std::string message = {
				buffers_begin(_readBuffer.data()),
				buffers_end(_readBuffer.data())
			};
	_readBuffer.consume(_readBuffer.size());

	if (message.empty()) {
		spdlog::error("TcpClient: Error: Empty string received");
	} else {
		std::stringstream ss;
		ss << "TcpClient: Message received: "
				<< message.c_str();
		spdlog::info(ss.str());
	}
	if (_readCallback) {
		_readCallback(message);
	}
	startRead();
}

void TcpClient::send(const std::string& message) noexcept {
	if (_stopped) {
		return;
	}
	std::stringstream ss;
	ss << "TcpClient. Sending message.\n\tMessage:"
			<< message
			<< "\n\tRemote host: "
			<< _socket.remote_endpoint().address().to_string()
			<< ":"
			<< _socket.remote_endpoint().port();
	spdlog::info(ss.str());
	async_write(_socket,
				asio::buffer(message),
				[this](const system::error_code& ec, std::size_t bytes) {
					if (ec) {
						std::stringstream ss;
						ss << "TcpClient. Error in writing.\nError: "
								<< ec.what().c_str();
						spdlog::error(ss.str());
						return;
					}
					handleWrite();
				});
}

void TcpClient::handleWrite() noexcept {
	if (_stopped) {
		return;
	}
}

#pragma region Accessors/Mutators

void TcpClient::readCallback(std::function<void(std::string)> callback) noexcept {
	_readCallback = std::move(callback);
}

bool TcpClient::isRunning() const noexcept {
	return !_stopped;
}

#pragma endregion Accessors/Mutators
