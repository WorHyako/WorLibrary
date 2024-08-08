#pragma once

#include <memory>

#include "boost/asio.hpp"

#include "Network/TcpSession.hpp"

namespace Wor::Network {

    /**
     * @brief
     *
     * @author WorHyako
     */
    class TcpServer {
    public:
        /**
         * @brief Ctor.
         */
        TcpServer() noexcept = default;

        /**
         * @brief Dtor.
         */
        virtual ~TcpServer() noexcept;

        /**
         * @brief
         *
         * @param ioService
         */
        void init(boost::asio::io_service &ioService) noexcept;

        /**
         * @brief
         *
         * @param endPoint
         */
        [[nodiscard]]
        bool bindTo(const boost::asio::ip::tcp::endpoint &endPoint) noexcept;

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
        void closeSession(const std::string &name) noexcept;

        /**
         * @brief
         *
         * @param message
         */
        void sendToAll(const std::string &message) const noexcept;

    private:
        /**
         * @brief           Closes selected session.
         *                  <p/>
         *                  Nothing happened, If session doesn't exist in _sessionList.
         *
         * @param session   Session ptr to close.
         */
        void closeSession(TcpSession::ptr session) noexcept;

        /**
         * @brief
         *
         * @param sessionPtr
         *
         * @param ec
         */
        void handleAccept(const TcpSession::ptr &sessionPtr, boost::system::error_code ec) noexcept;

        std::unique_ptr<boost::asio::ip::tcp::acceptor> _acceptor;

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
        TcpSession::ptr session(const std::string &name) noexcept;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        std::vector<TcpSession::ptr> sessionList() const noexcept;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        bool isRunning() const noexcept;

#pragma endregion Accessors/Mutators
    };
}
