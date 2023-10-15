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
        virtual void CloseConnection() noexcept = 0;

        /**
         *
         * @return
         */
        [[nodiscard]] virtual bool TryToConnect() noexcept = 0;

        /**
         *
         * @param message_
         */
        [[nodiscard]] virtual std::size_t Send(const std::string &message_) noexcept = 0;

        /**
         *
         * @return
         */
        [[nodiscard]] virtual SocketStatus Status() noexcept = 0;
    };
}
