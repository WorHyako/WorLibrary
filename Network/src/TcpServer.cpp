#include "TcpServer.hpp"

#include "TcpSession.hpp"

#include <cstdio>

#include "Network/Utils/IoService.hpp"

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

    auto &ioService = Utils::IoService::get();
    auto newSession = TcpSession::create(ioService);
    newSession->closeCallback([this](const TcpSession::ptr& session) {
        closeSession(session);
    });
    newSession->name("session_" + std::to_string(_sessionsList.size()));

    _acceptor->async_accept(newSession->socket(),
                            [this, newSession](const system::error_code &ec) {
                                if (!ec) {
                                    handleAccept(newSession, ec);
                                } else {
                                    closeSession(newSession);
//                                   auto it = std::find(_sessionsList.begin(), _sessionsList.end(), newSession);
//                                   std::ignore = _sessionsList.erase(it);
//                                   auto it = std::remove(_sessionsList.begin(), _sessionsList.end(), newSession);
//                                   it->reset();
                                }
                            });
}

void TcpServer::stop() noexcept {
    if (!_acceptor || !_acceptor->is_open()) {
        return;
    }
    _acceptor->cancel();
    std::ranges::for_each(_sessionsList, [](TcpSession::ptr session) {
        session->close();
        session.reset();
    });
    _sessionsList.clear();
    _acceptor->close();

    _acceptor.reset();
}

void TcpServer::closeSession(const TcpSession::ptr& session) noexcept {
    if (!session.get()) {
        return;
    }
    session->close();
    const auto position = std::ranges::remove(_sessionsList, session).begin();
    if(position == _sessionsList.end()) {
        return;
    }
    const auto t = _sessionsList.erase(position);

    std::printf("TcpServer::closeSession:\n\tsession: %s", session->name().c_str());
    t->reset();
}

void TcpServer::closeSession(const std::string &name) noexcept {
    const TcpSession::ptr session = TcpServer::session(name);
    if (session == nullptr) {
        return;
    }
    session->close();
    const auto it = _sessionsList.erase(std::ranges::remove(_sessionsList, session).begin());
    if (it == std::end(_sessionsList)) {
        return;
    }
    it->reset();
}

void TcpServer::handleAccept(const TcpSession::ptr &sessionPtr, system::error_code ec) noexcept {
    if (!ec) {
        const tcp::endpoint remoteEndpoint = sessionPtr->socket().remote_endpoint();
        std::printf("New session accepted.\n \tRemote endpoint address: %s:%s\n",
                    remoteEndpoint.address().to_string().c_str(),
                    std::to_string(remoteEndpoint.port()).c_str());
        sessionPtr->run();
        _sessionsList.emplace_back(sessionPtr);
    } else {
        std::printf("Error in connection accepting.\n\tError: %s", ec.what().c_str());
    }

    start();
}

void TcpServer::sendToAll(const std::string &message) const noexcept {
    std::ranges::for_each(_sessionsList, [&message](const TcpSession::ptr &session) {
        session->send(message);
    });
}

bool TcpServer::bindTo(const tcp::endpoint &endPoint) noexcept {
    stop();
    auto &ioService = Utils::IoService::get();
    _acceptor.reset(new tcp::acceptor(ioService));
    std::printf("TcpServer::bindTo:\n\tStart binding.\n\tAddress: %s:%i\n",
                endPoint.address().to_string().c_str(), endPoint.port());

    system::error_code ec;
    ec = _acceptor->open(endPoint.protocol(), ec);
    if (ec) {
        std::printf("TcpServer::bindTo:\n\topen: %s\n", ec.what().c_str());
        return false;
    }
    _acceptor->set_option(tcp::acceptor::reuse_address(true));
    ec = _acceptor->bind(endPoint, ec);
    if (ec) {
        std::printf("TcpServer::bindTo:\n\tbindTo: %s\n", ec.what().c_str());
        return false;
    }
    ec = _acceptor->listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        std::printf("TcpServer::bindTo:\n\tlisten: %s\n", ec.what().c_str());
        return false;
    }
    std::printf("TcpServer::bindTo:\n\tSuccessful binding.\n");
    return true;
}

#pragma region Accessors/Mutators

TcpSession::ptr TcpServer::session(const std::string &name) noexcept {
    const auto session = std::ranges::find_if(_sessionsList,
                                              [&name](const TcpSession::ptr &each) {
                                                  return each->name() == name;
                                              });
    return session == std::end(_sessionsList) ? nullptr : *session;
}

std::vector<TcpSession::ptr> TcpServer::sessionList() const noexcept {
    return _sessionsList;
}

bool TcpServer::isRunning() const noexcept {
    return _acceptor->is_open();
}

#pragma endregion Accessors/Mutators
