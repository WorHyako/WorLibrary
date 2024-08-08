#pragma once

#include "boost/asio.hpp"

#include <memory>
#include <functional>

namespace Wor::Network {

    /**
     * @brief
     *
     * @author WorHyako
     */
    class TcpSession final
            : public std::enable_shared_from_this<TcpSession>,
              public boost::asio::noncopyable {
    public:
        typedef std::shared_ptr<TcpSession> ptr;

        /**
         * @brief
         */
        void run() noexcept;

        /**
         * @brief
         *
         * @param ioCtx
         *
         * @return
         */
        [[nodiscard]]
        static TcpSession::ptr create(boost::asio::io_service& ioService) noexcept;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        boost::asio::ip::tcp::socket &socket() noexcept;

        /**
         * @brief
         */
        void close();

        /**
         * @brief
         *
         * @param message
         */
        void send(const std::string& message) noexcept;

    private:
        /**
         * @brief Ctor.
         *
         * @param ioContext
         */
        explicit TcpSession(boost::asio::io_service& ioContext) noexcept;

        /**
         * @brief
         */
        void startReading() noexcept;

        /**
         * @brief
         */
        void parseBuffer() noexcept;

        boost::asio::ip::tcp::socket _socket;

        boost::asio::streambuf _buffer;

        std::string _name;

        std::function<void(TcpSession::ptr)> _closeCallback;

        bool _isActive;

    public:
#pragma region Accessors/Mutators

        /**
         * @brief
         *
         * @param name
         */
        void name(std::string name) noexcept;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        std::string name() noexcept;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        boost::asio::ip::tcp::endpoint endpoint() const noexcept;

        /**
         * @brief
         *
         * @param callback
         */
        void closeCallback(std::function<void(TcpSession::ptr)> callback) noexcept;

#pragma endregion Accessors/Mutators
    };
}
