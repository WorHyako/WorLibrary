#include "Network/TcpServer.hpp"

#include "Network/TcpSession.hpp"
#include "Network/Utils/IoService.hpp"

#include "Wor/Log/Log.hpp"

using namespace Wor::Network;
using namespace boost;

TcpServer::~TcpServer() noexcept {
	stop();
}

void TcpServer::start() noexcept {
	if (!bound()) {
		return;
	}

	auto& io_service{Utils::IoService::get()};
	auto new_session{TcpSession::create(io_service)};
	new_session->closeCallback([this](const TcpSession::ptr& session) {
		closeSession(session);
	});
	new_session->alias("session_" + std::to_string(_sessions_list.size()));

	_acceptor->async_accept(new_session->socket(),
							[this, new_session](const system::error_code& ec) {
								if (ec) {
									closeSession(new_session);
								} else {
									worError("Session accepting error: {}", ec.message());
									handleAccept(new_session, ec);
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
	} catch (const system::system_error& e) {
		worError("Error in acceptor canceling: {}.", e.what());
	}
	std::ranges::for_each(_sessions_list,
						  [](TcpSession::ptr session) {
							  session->close();
							  session.reset();
						  });
	_sessions_list.clear();
	_receive_callbacks.clear();

	try {
		_acceptor->close();
	} catch (const system::system_error& e) {
		worError("Error in acceptor closing: {}. Descriptor was closed.", e.what());
	}

	_acceptor.reset();
	worInfo("Server was stopped.");
}

void TcpServer::closeSession(const TcpSession::ptr& session) noexcept {
	if (!session.get()) {
		return;
	}
	worInfo("Closing session: {} - {}:{}",
			session->alias().c_str(),
			session->endpoint().address().to_string(),
			session->endpoint().port());

	session->close();
	const auto position{std::begin(std::ranges::remove(_sessions_list, session))};
	if (position == _sessions_list.end()) {
		return;
	}
	const auto trash{_sessions_list.erase(position)};
	trash->reset();
}

void TcpServer::closeSession(const std::string_view& alias) noexcept {
	const TcpSession::ptr session{TcpServer::session(alias)};
	closeSession(session);
}

void TcpServer::closeSession(const Endpoint& endpoint) noexcept {
	const TcpSession::ptr session{TcpServer::session(endpoint)};
	closeSession(session);
}

void TcpServer::handleAccept(const TcpSession::ptr& session_ptr, system::error_code ec) noexcept {
	Endpoint remote_endpoint{session_ptr->socket().remote_endpoint()};
	if (ec) {
		worError("Error in connection accepting.\nAlias: {}\nRemote endpoint: {}:{}",
				 session_ptr->alias(),
				 remote_endpoint.address().to_string(),
				 remote_endpoint.port());
	} else {
		worInfo("New session was accepted.\nAlias: {}\nRemote endpoint: {}:{}",
				session_ptr->alias(),
				remote_endpoint.address().to_string(),
				remote_endpoint.port());
		session_ptr->run();
		_sessions_list.emplace_back(session_ptr);
		auto callback_pair{
					std::ranges::find_if(_receive_callbacks,
										 [&remote_endpoint](const std::pair<Endpoint, Callback>& each) {
											 return each.first == remote_endpoint;
										 })
				};
		if (callback_pair != std::end(_receive_callbacks)) {
			session_ptr->receiveCallback(callback_pair->second);
		}
	}

	start();
}

void TcpServer::sendToAll(const std::string_view& message) const noexcept {
	std::ranges::for_each(_sessions_list,
						  [&message](const TcpSession::ptr& session) {
							  session->send(message);
						  });
}

bool TcpServer::bindTo(const Endpoint& endpoint) noexcept {
	stop();
	auto& io_service{Utils::IoService::get()};
	_acceptor = std::make_unique<Acceptor>(io_service);

	worInfo("Binding to {}:{}",
			endpoint.address().to_string(),
			endpoint.port());
	system::error_code ec;
	std::ignore = _acceptor->open(endpoint.protocol(), ec);
	if (ec) {
		worError("Open port error: {}", ec.message());
		return false;
	}
	_acceptor->set_option(Acceptor::reuse_address(true));
	std::ignore = _acceptor->bind(endpoint, ec);
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

TcpSession::ptr TcpServer::session(const std::string_view& alias) noexcept {
	const auto session{
				std::ranges::find_if(_sessions_list,
									 [&alias](const TcpSession::ptr& each) {
										 return each->alias() == alias;
									 })
			};
	return session == std::end(_sessions_list)
			   ? nullptr
			   : *session;
}

TcpSession::ptr TcpServer::session(const Endpoint& endpoint) noexcept {
	const auto session{
				std::ranges::find_if(_sessions_list,
									 [&endpoint](const TcpSession::ptr& each) {
										 return each->endpoint() == endpoint;
									 })
			};
	return session == std::end(_sessions_list)
			   ? nullptr
			   : *session;
}

std::vector<TcpSession::ptr> TcpServer::sessionList() const noexcept {
	return _sessions_list;
}

bool TcpServer::bound() const noexcept {
	return _acceptor
			   ? _acceptor->is_open()
			   : false;
}

void TcpServer::receiveCallback(Endpoint endpoint, Callback callback) noexcept {
	_receive_callbacks.emplace(endpoint, std::move(callback));

	auto session{
				std::ranges::find_if(_sessions_list,
									 [&endpoint](const TcpSession::ptr& each) {
										 return each->endpoint() == endpoint;
									 })
			};
	if (session == std::end(_sessions_list)) {
		return;
	}
	(*session)->receiveCallback(callback);
}

#pragma endregion Accessors/Mutators
