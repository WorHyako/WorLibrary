#include "WorLibrary/Network/TcpServer.hpp"

#include "WorLibrary/Network/TcpSession.hpp"

#include <cstdio>

using namespace Wor::Network;

using namespace boost::asio::ip;

TcpServer::TcpServer(boost::asio::io_context &ioContext, std::uint16_t port) noexcept
        : _ioService(ioContext),
          _acceptor(ioContext, tcp::endpoint(tcp::v4(), port)) {
}

void TcpServer::start() noexcept {
    auto newSession = TcpSession::create(_ioService);
    newSession->setName("session_" + std::to_string(_sessionsList.size()));

    _acceptor.async_accept(newSession->socket(),
                           [this, newSession](const boost::system::error_code &ec) {
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
    std::for_each(std::begin(_sessionsList), std::end(_sessionsList), [](TcpSession::ptr session){
        session->close();
        session.reset();
    });
    _sessionsList.clear();
    _acceptor.close();
}

void TcpServer::closeSession(const std::string &name) noexcept {
    auto session = getSession(name);
    if(session ==nullptr) {
        return;
    }
    session->close();
    auto it = std::remove(_sessionsList.begin(), _sessionsList.end(), session);
    it->reset();
}

void TcpServer::handleAccept(const TcpSession::ptr &sessionPtr, boost::system::error_code ec) noexcept {
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

void TcpServer::sendToAll(const std::string &message) noexcept {
    std::for_each(std::begin(_sessionsList), std::end(_sessionsList), [&message](TcpSession::ptr session){
        session->send(message);
    });
}
