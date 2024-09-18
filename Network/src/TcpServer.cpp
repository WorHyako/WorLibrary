#include "Network/TcpServer.hpp"

#include "Network/TcpSession.hpp"
#include "Network/Utils/IoService.hpp"

#include <spdlog/spdlog.h>

using namespace Wor::Network;

using namespace boost::asio::ip;
using namespace boost;

TcpServer::~TcpServer() noexcept {
	stop();
}

void TcpServer::start() noexcept {
	if (!_acceptor || !_acceptor->is_open()) {
		return;
	}

	auto& ioService = Utils::IoService::get();
	auto newSession = TcpSession::create(ioService);
	newSession->closeCallback([this](const TcpSession::ptr& session) {
		closeSession(session);
	});
	newSession->name("session_" + std::to_string(_sessionsList.size()));

	_acceptor->async_accept(newSession->socket(),
							[this, newSession](const system::error_code& ec) {
								if (!ec) {
									handleAccept(newSession, ec);
								} else {
									closeSession(newSession);
									// auto it = std::find(_sessionsList.begin(), _sessionsList.end(), newSession);
									// std::ignore = _sessionsList.erase(it);
									// auto it = std::remove(_sessionsList.begin(), _sessionsList.end(), newSession);
									// it->reset();
								}
							});
}

void TcpServer::stop() noexcept {
	if (!_acceptor || !_acceptor->is_open()) {
		return;
	}
	_acceptor->cancel();
	std::ranges::for_each(_sessionsList,
						  [](TcpSession::ptr session) {
							  session->close();
							  session.reset();
						  });
	_sessionsList.clear();
	_receiveCallbacks.clear();
	_acceptor->close();

	_acceptor.reset();
}

void TcpServer::closeSession(const TcpSession::ptr& session) noexcept {
	if (!session.get()) {
		return;
	}
	spdlog::info("TcpServer: Closing session: {}", session->name().c_str());

	session->close();
	const auto position = std::begin(std::ranges::remove(_sessionsList, session));
	if (position == _sessionsList.end()) {
		return;
	}
	const auto trash = _sessionsList.erase(position);
	trash->reset();
}

void TcpServer::closeSession(const std::string& name) noexcept {
	const TcpSession::ptr session = TcpServer::session(name);
	closeSession(session);
}

void TcpServer::closeSession(const tcp::endpoint& endpoint) noexcept {
	const TcpSession::ptr session = TcpServer::session(endpoint);
	closeSession(session);
}

void TcpServer::handleAccept(const TcpSession::ptr& sessionPtr, system::error_code ec) noexcept {
	tcp::endpoint remoteEndpoint = sessionPtr->socket().remote_endpoint();
	if (!ec) {
		std::stringstream ss;
		ss << "TcpServer: New session was accepted.\nRemote endpoint: "
				<< remoteEndpoint.address().to_string().c_str()
				<< ":"
				<< remoteEndpoint.port();
		spdlog::info(ss.str());
		sessionPtr->run();
		_sessionsList.emplace_back(sessionPtr);
		auto callbackPair =
				std::ranges::find_if(_receiveCallbacks,
									 [&remoteEndpoint](const std::pair<tcp::endpoint,
																	   std::function<void(const std::string&)>>& each) {
										 return each.first == remoteEndpoint;
									 });
		if (callbackPair != std::end(_receiveCallbacks)) {
			sessionPtr->receiveCallback(callbackPair->second);
		}
	} else {
		std::stringstream ss;
		ss << "TcpServer: Error in connection accepting.\nRemote endpoint: "
				<< remoteEndpoint.address().to_string().c_str()
				<< ":"
				<< remoteEndpoint.port()
				<< "\nError: "
				<< ec.what().c_str();
		spdlog::error(ss.str());
	}

	start();
}

void TcpServer::sendToAll(const std::string& message) const noexcept {
	std::ranges::for_each(_sessionsList,
						  [&message](const TcpSession::ptr& session) {
							  session->send(message);
						  });
}

bool TcpServer::bindTo(const tcp::endpoint& endPoint) noexcept {
	stop();
	auto& ioService = Utils::IoService::get();
	_acceptor = std::make_unique<tcp::acceptor>(ioService);
	std::stringstream ss;
	ss << "TcpServer. Start binding to: "
			<< endPoint.address().to_string().c_str()
			<< ":"
			<< endPoint.port();
	spdlog::info(ss.str());

	system::error_code ec;
	std::ignore = _acceptor->open(endPoint.protocol(), ec);
	if (ec) {
		spdlog::error("TcpServer. Open port error: {}", ec.what().c_str());
		return false;
	}
	_acceptor->set_option(tcp::acceptor::reuse_address(true));
	std::ignore = _acceptor->bind(endPoint, ec);
	if (ec) {
		spdlog::error("TcpServer. Binding error: {}", ec.what().c_str());
		return false;
	}
	std::ignore = _acceptor->listen(asio::socket_base::max_listen_connections, ec);
	if (ec) {
		spdlog::error("TcpServer. Listening error: {}", ec.what().c_str());
		return false;
	}
	spdlog::info("TcpServer. Successful binding");
	return true;
}

#pragma region Accessors/Mutators

TcpSession::ptr TcpServer::session(const std::string& name) noexcept {
	const auto session = std::ranges::find_if(_sessionsList,
											  [&name](const TcpSession::ptr& each) {
												  return each->name() == name;
											  });
	return session == std::end(_sessionsList) ? nullptr : *session;
}

TcpSession::ptr TcpServer::session(const tcp::endpoint& endpoint) noexcept {
	const auto session = std::ranges::find_if(_sessionsList,
											  [&endpoint](const TcpSession::ptr& each) {
												  return each->endpoint() == endpoint;
											  });
	return session == std::end(_sessionsList) ? nullptr : *session;
}

std::vector<TcpSession::ptr> TcpServer::sessionList() const noexcept {
	return _sessionsList;
}

bool TcpServer::isRunning() const noexcept {
	return _acceptor->is_open();
}

void TcpServer::receiveCallback(tcp::endpoint endpoint, std::function<void(const std::string&)> callback) noexcept {
	_receiveCallbacks.emplace(endpoint, std::move(callback));

	auto session = std::ranges::find_if(_sessionsList,
										[&endpoint](const TcpSession::ptr& each) {
											return each->endpoint() == endpoint;
										});
	if (session == std::end(_sessionsList)) {
		return;
	}
	(*session)->receiveCallback(callback);
}

#pragma endregion Accessors/Mutators
