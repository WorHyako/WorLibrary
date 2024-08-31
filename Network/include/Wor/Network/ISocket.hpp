#pragma once

#include <string>

namespace Wor::Network {

    /**
     * @brief
     *
     * @author WorHyako
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
     * @brief
     *
     * @author WorHyako
     */
    class ISocket {
    public:
        /**
         * @brief Ctor.
         */
        ISocket() = default;

        /**
         * @brief Dtor.
         */
        virtual ~ISocket() noexcept = default;

        /**
         *
         */
        virtual void closeConnection() noexcept = 0;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        virtual bool tryToConnect() noexcept = 0;

        /**
         * @brief
         *
         * @param message_
         */
        [[nodiscard]]
        virtual std::size_t send(const std::string &message) noexcept = 0;

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        virtual SocketStatus status() noexcept = 0;
    };
}
