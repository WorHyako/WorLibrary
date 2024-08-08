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

    auto &ioService = Wor::Network::Utils::IoService::get();
    auto newSession = TcpSession::create(ioService);
    newSession->closeCallback([this](TcpSession::ptr session) {
        closeSession(session);
    });
    newSession->name("session_" + std::to_string(_sessionsList.size()));

    _acceptor->async_accept(newSession->socket(),
                            [this, newSession](const system::error_code &ec) {
                                std::printf("\n**********CLOSE*********\n");
                                if (!ec) {
                                    handleAccept(newSession, ec);
                                } else {
                                    newSession->close();
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
    std::for_each(std::begin(_sessionsList), std::end(_sessionsList), [](TcpSession::ptr session) {
        session->close();
        session.reset();
    });
    _sessionsList.clear();
    _acceptor->close();

    _acceptor.reset();
}

void TcpServer::closeSession(TcpSession::ptr session) noexcept {
    if (!session.get()) {
        return;
    }
    session->close();
    auto t = std::remove(_sessionsList.begin(), _sessionsList.end(), session);
    if (t == std::end(_sessionsList)) {
        return;
    }
    std::printf("TcpServer::closeSession:\n\tsession: %s", session->name().c_str());
    t->reset();
}

void TcpServer::closeSession(const std::string &name) noexcept {
    TcpSession::ptr session = TcpServer::session(name);
    if (session == nullptr) {
        return;
    }
    session->close();
    auto it = std::remove(_sessionsList.begin(), _sessionsList.end(), session);
    it->reset();
}

void TcpServer::handleAccept(const TcpSession::ptr &sessionPtr, system::error_code ec) noexcept {
    if (!ec) {
        tcp::endpoint remoteEndpoint = sessionPtr->socket().remote_endpoint();
        std::printf("New session accepted.\n \tRemote endpoint address: %s:%s\n",
                    remoteEndpoint.address().to_string().c_str(),
                    std::to_string(remoteEndpoint.port()).c_str());
        sessionPtr->send("Connection approved.");
        sessionPtr->run();
        _sessionsList.emplace_back(sessionPtr);
    } else {
        std::printf("Error in connection accepting.\n\tError: %s", ec.what().c_str());
    }

    start();
}

void TcpServer::sendToAll(const std::string &message) const noexcept {
    std::for_each(std::begin(_sessionsList), std::end(_sessionsList), [&message](const TcpSession::ptr &session) {
        session->send(message);
    });
}

bool TcpServer::bindTo(const tcp::endpoint &endPoint) noexcept {
    if (!_acceptor) {
        auto &ioService = Wor::Network::Utils::IoService::get();
        _acceptor = std::make_unique<tcp::acceptor>(ioService);
    }
    std::printf("TcpServer::bindTo:\n\tStart binding.\n\tAddress: %s:%i\n",
                endPoint.address().to_string().c_str(), endPoint.port());
    if (_acceptor->is_open()) {
        stop();
    }
    system::error_code ec;
    ec = _acceptor->open(endPoint.protocol(), ec);
    if (ec) {
        std::printf("TcpServer::bindTo:\n\topen: %s\n", ec.what().c_str());
        return false;
    }
    _acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
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
    start();
    std::printf("TcpServer::bindTo:\n\tSuccessful binding.\n");
    return true;
}

#pragma region Accessors/Mutators

TcpSession::ptr TcpServer::session(const std::string &name) noexcept {
    auto session = std::find_if(std::begin(_sessionsList), std::end(_sessionsList),
                                [&name](const TcpSession::ptr &session) {
                                    return session->name() == name;
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
