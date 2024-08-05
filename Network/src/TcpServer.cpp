#include "TcpServer.hpp"

#include "TcpSession.hpp"

#include <cstdio>

using namespace Wor::Network;

using namespace boost::asio::ip;
using namespace boost;

TcpServer::TcpServer(asio::io_service& ioService) noexcept {
    _ioService.reset(&ioService);
    _acceptor = std::make_unique<asio::ip::tcp::acceptor>(ioService);
}

void TcpServer::start() noexcept {
    if (!_acceptor->is_open()) {
        return;
    }
    auto newSession = TcpSession::create(_ioService);
    newSession->setName("session_" + std::to_string(_sessionsList.size()));

    _acceptor->async_accept(newSession->socket(),
                           [this, newSession](const system::error_code &ec) {
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
    if (!_acceptor->is_open()) {
        return;
    }
    std::for_each(std::begin(_sessionsList), std::end(_sessionsList), [](TcpSession::ptr session) {
        session->close();
        session.reset();
    });
    _sessionsList.clear();
    _acceptor->close();
}

void TcpServer::closeSession(const std::string &name) noexcept {
    auto session = getSession(name);
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

TcpSession::ptr TcpServer::getSession(const std::string &name) noexcept {
    auto session = std::find_if(std::begin(_sessionsList), std::end(_sessionsList), [&name](TcpSession::ptr &session) {
        return session->getName() == name;
    });
    return session == std::end(_sessionsList) ? nullptr : *session;
}

void TcpServer::sendToAll(const std::string &message) const noexcept {
    std::for_each(std::begin(_sessionsList), std::end(_sessionsList), [&message](const TcpSession::ptr &session) {
        session->send(message);
    });
}

void TcpServer::bindTo(const boost::asio::ip::tcp::endpoint &endPoint) noexcept {
    if (_acceptor->is_open()) {
        stop();
    }
    _acceptor->open(endPoint.protocol());
    _acceptor->bind(endPoint);
    _acceptor->listen();
}

void TcpServer::init(asio::io_service& ioService) noexcept {
    _ioService.reset(&ioService);
    _acceptor = std::make_unique<asio::ip::tcp::acceptor>(ioService);
}
