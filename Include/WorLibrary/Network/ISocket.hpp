#pragma once

#include <string>

namespace Wor::Network {

    /**
     *
     */
    enum class SocketStatus {
        /**
         *
         */
        ZeroCheck,
        /**
         *
         */
        Connected,
        /**
         *
         */
        Reachable,
        /**
         *
         */
        Unreachable
    };

    /**
     *
     */
    class ISocket {
    public:
        /**
         * Ctor
         */
        ISocket() = default;

        /**
         * Dtor
         */
        virtual ~ISocket() noexcept = default;

        /**
         *
         */
        virtual void closeConnection() noexcept = 0;

        /**
         *
         * @return
         */
        [[nodiscard]] virtual bool tryToConnect() noexcept = 0;

        /**
         *
         * @param message_
         */
        [[nodiscard]] virtual std::size_t send(const std::string &message_) noexcept = 0;

        /**
         *
         * @return
         */
        [[nodiscard]] virtual SocketStatus status() noexcept = 0;
    };
}
