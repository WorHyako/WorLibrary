#include "Network/TcpSocket.hpp"

#include "Network/Utils/EndPointChecker.hpp"

using namespace Wor::Network;

namespace {

	/**
	 * Boost socket context
	 */
	boost::asio::io_context context;
}

TcpSocket::TcpSocket() noexcept
	: _socket(::context),
	  _status(SocketStatus::ZeroCheck) {
}

TcpSocket::~TcpSocket() noexcept {
	if (_status == SocketStatus::Connected) {
		closeSocket();
		_status = SocketStatus::ZeroCheck;
	}
}

void TcpSocket::cleanErrors() noexcept {
	_errors.clear();
}

std::size_t TcpSocket::send(const std::string &message) noexcept {
	std::size_t bytesSent = 0;
	if (_status != SocketStatus::Connected) {
		return bytesSent;
	}
	const auto buf = boost::asio::buffer(message, static_cast<std::size_t>(message.size()));
	bytesSent = _socket.send(buf);
	return bytesSent;
}

bool TcpSocket::tryToConnect() noexcept {
	_status = SocketStatus::Unreachable;
	auto destinationEndPoint = AsioTcpEndPoint(
			boost::asio::ip::address::from_string(_destinationEndPoint._address.c_str()),
			_destinationEndPoint._port);
	auto asyncConnect = std::async(std::launch::async,
								   [this, &destinationEndPoint]() {
									   _socket.connect(destinationEndPoint);
								   });
	try {
		auto timeout = asyncConnect.wait_for(std::chrono::milliseconds(100));
		if (timeout == std::future_status::timeout) {
			_socket.cancel();
			_socket.close();
			return false;
		}
	} catch (std::exception &e) {
		/// Temporary. Just for debug
		std::string errorMessage = e.what();
		return false;
	}
	_status = SocketStatus::Connected;
	return true;
}

void TcpSocket::closeConnection() noexcept {
	closeSocket();
}

bool TcpSocket::checkEndPoint(const std::string &address) noexcept {
	return Utils::checkEndPoint(address);
}

void TcpSocket::closeSocket() noexcept {
	if (_socket.is_open()) {
		//        boost::system::error_code ec;
		//        auto sec = _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
		auto t = _socket.available();
		_socket.close();
	}
	_status = SocketStatus::Reachable;
}

#pragma region Accessors

const EndPoint &TcpSocket::destinationEndPoint() const noexcept {
	return _destinationEndPoint;
}

const EndPoint &TcpSocket::sourceEndPoint() const noexcept {
	return _sourceEndPoint;
}

std::string TcpSocket::getLastError() const noexcept {
	return _errors.back().message();
}

const std::vector<TcpSocket::BoostErrorCode> &TcpSocket::getErrors() const noexcept {
	return _errors;
}

SocketStatus TcpSocket::status() noexcept {
	return _status;
}

#pragma endregion Accessors

#pragma region Mutators

void TcpSocket::destinationEndPoint(EndPoint endPoint) noexcept {
	_destinationEndPoint = std::move(endPoint);
}

void TcpSocket::sourceEndPoint(EndPoint endPoint) noexcept {
	_sourceEndPoint = std::move(endPoint);
}

#pragma endregion Mutators
