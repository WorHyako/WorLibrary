#include "Network/TcpServer.hpp"

#include "Network/TcpSession.hpp"
#include "Network/Utils/IoService.hpp"

#include "Wor/Log/Log.hpp"

using namespace Wor::Network;

using namespace boost::asio::ip;
using namespace boost;

TcpServer::~TcpServer() noexcept {
	stop();
}

void TcpServer::start() noexcept {
	if (!bound()) {
		return;
	}

	auto &ioService = Utils::IoService::get();
	auto newSession = TcpSession::create(ioService);
	newSession->closeCallback([this](const TcpSession::ptr &session) {
		closeSession(session);
	});
	newSession->alias("session_" + std::to_string(_sessionsList.size()));

	_acceptor->async_accept(newSession->socket(),
							[this, newSession](const system::error_code &ec) {
								if (ec) {
									closeSession(newSession);
								} else {
									worError("Session accepting error: {}", ec.message());
									handleAccept(newSession, ec);
									// auto it = std::find(_sessionsList.begin(), _sessionsList.end(), newSession);
									// std::ignore = _sessionsList.erase(it);
									// auto it = std::remove(_sessionsList.begin(), _sessionsList.end(), newSession);
									// it->reset();
								}
							});
}

void TcpServer::stop() noexcept {
	if (!bound()) {
		return;
	}

	try {
		_acceptor->cancel();
	} catch (const system::system_error &e) {
		worError("Error in acceptor canceling: {}.", e.what());
	}
	std::ranges::for_each(_sessionsList,
						  [](TcpSession::ptr session) {
							  session->close();
							  session.reset();
						  });
	_sessionsList.clear();
	_receiveCallbacks.clear();

	try {
		_acceptor->close();
	} catch (const system::system_error &e) {
		worError("Error in acceptor closing: {}. Descriptor was closed.", e.what());
	}

	_acceptor.reset();
	worInfo("Server was stopped.");
}

void TcpServer::closeSession(const TcpSession::ptr &session) noexcept {
	if (!session.get()) {
		return;
	}
	worInfo("Closing session: {} - {}:{}",
			session->alias().c_str(),
			session->endpoint().address().to_string(),
			session->endpoint().port());

	session->close();
	const auto position = std::begin(std::ranges::remove(_sessionsList, session));
	if (position == _sessionsList.end()) {
		return;
	}
	const auto trash = _sessionsList.erase(position);
	trash->reset();
}

void TcpServer::closeSession(const std::string &alias) noexcept {
	const TcpSession::ptr session = TcpServer::session(alias);
	closeSession(session);
}

void TcpServer::closeSession(const Endpoint &endpoint) noexcept {
	const TcpSession::ptr session = TcpServer::session(endpoint);
	closeSession(session);
}

void TcpServer::handleAccept(const TcpSession::ptr &sessionPtr, system::error_code ec) noexcept {
	Endpoint remoteEndpoint = sessionPtr->socket().remote_endpoint();
	if (ec) {
		worError("Error in connection accepting.\nAlias: {}\nRemote endpoint: {}:{}",
				 sessionPtr->alias(),
				 remoteEndpoint.address().to_string(),
				 remoteEndpoint.port());
	} else {
		worInfo("New session was accepted.\nAlias: {}\nRemote endpoint: {}:{}",
				sessionPtr->alias(),
				remoteEndpoint.address().to_string(),
				remoteEndpoint.port());
		sessionPtr->run();
		_sessionsList.emplace_back(sessionPtr);
		auto callbackPair =
				std::ranges::find_if(_receiveCallbacks,
									 [&remoteEndpoint](const std::pair<Endpoint, Callback> &each) {
										 return each.first == remoteEndpoint;
									 });
		if (callbackPair != std::end(_receiveCallbacks)) {
			sessionPtr->receiveCallback(callbackPair->second);
		}
	}

	start();
}

void TcpServer::sendToAll(const std::string &message) const noexcept {
	std::ranges::for_each(_sessionsList,
						  [&message](const TcpSession::ptr &session) {
							  session->send(message);
						  });
}

bool TcpServer::bindTo(const Endpoint &endPoint) noexcept {
	stop();
	auto &ioService = Utils::IoService::get();
	_acceptor = std::make_unique<Acceptor>(ioService);

	worInfo("Binding to {}:{}",
			endPoint.address().to_string(),
			endPoint.port());
	system::error_code ec;
	std::ignore = _acceptor->open(endPoint.protocol(), ec);
	if (ec) {
		worError("Open port error: {}", ec.message());
		return false;
	}
	_acceptor->set_option(Acceptor::reuse_address(true));
	std::ignore = _acceptor->bind(endPoint, ec);
	if (ec) {
		worError("Binding error: {}", ec.message());
		return false;
	}
	std::ignore = _acceptor->listen(asio::socket_base::max_listen_connections, ec);
	if (ec) {
		worError("Listening error: {}", ec.message());
		stop();
		return false;
	}
	worTrace("Success.");
	return true;
}

#pragma region Accessors/Mutators

TcpSession::ptr TcpServer::session(const std::string &alias) noexcept {
	const auto session = std::ranges::find_if(_sessionsList,
											  [&alias](const TcpSession::ptr &each) {
												  return each->alias() == alias;
											  });
	return session == std::end(_sessionsList) ? nullptr : *session;
}

TcpSession::ptr TcpServer::session(const Endpoint &endpoint) noexcept {
	const auto session = std::ranges::find_if(_sessionsList,
											  [&endpoint](const TcpSession::ptr &each) {
												  return each->endpoint() == endpoint;
											  });
	return session == std::end(_sessionsList) ? nullptr : *session;
}

std::vector<TcpSession::ptr> TcpServer::sessionList() const noexcept {
	return _sessionsList;
}

bool TcpServer::bound() const noexcept {
	return _acceptor ? _acceptor->is_open() : false;
}

void TcpServer::receiveCallback(Endpoint endpoint, Callback callback) noexcept {
	_receiveCallbacks.emplace(endpoint, std::move(callback));

	auto session = std::ranges::find_if(_sessionsList,
										[&endpoint](const TcpSession::ptr &each) {
											return each->endpoint() == endpoint;
										});
	if (session == std::end(_sessionsList)) {
		return;
	}
	(*session)->receiveCallback(callback);
}

#pragma endregion Accessors/Mutators
