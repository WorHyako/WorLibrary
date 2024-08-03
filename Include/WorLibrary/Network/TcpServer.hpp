#pragma once

#include <memory>

#include "boost/asio.hpp"

#include "WorLibrary/Network/TcpSession.hpp"

namespace Wor::Network {

    /**
     * @brief
     *
     * @author WorHyako
     */
    class TcpServer {
    public:
        /**
         * @brief
         *
         * @param ioContext
         */
        TcpServer(boost::asio::io_service &ioContext, std::uint16_t port) noexcept;

        /**
         * @brief
         *
         * @return
         */
        void start() noexcept;

        /**
         * @brief
         */
        void stop() noexcept;

        /**
         * @brief
         *
         * @param name
         */
        void closeSession(const std::string& name) noexcept;

        /**
         * @brief
         *
         * @param message
         */
        void sendToAll(const std::string& message) noexcept;

    private:
        /**
         * @brief
         *
         * @param sessionPtr
         *
         * @param ec
         */
        void handleAccept(const TcpSession::ptr& sessionPtr, boost::system::error_code ec) noexcept;

        boost::asio::io_service &_ioService;

        boost::asio::ip::tcp::acceptor _acceptor;

        std::vector<TcpSession::ptr> _sessionsList;

    public:
#pragma region Accessors/Mutators

        /**
         * @brief
         *
         * @param name
         *
         * @return
         */
        [[nodiscard]]
        TcpSession::ptr getSession(const std::string& name) noexcept;

#pragma endregion Accessors/Mutators
    };
}
