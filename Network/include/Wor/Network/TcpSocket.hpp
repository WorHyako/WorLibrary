#pragma once

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <memory>

#include "Network/EndPoint.hpp"
#include "Network/ISocket.hpp"

namespace Wor::Network {

    /**
     * @brief
     *
     * @author WorHyako
     */
    class TcpSocket
            : public ISocket {
    public:
        using AsioTcp = boost::asio::ip::tcp;
        using AsioTcpEndPoint = AsioTcp::endpoint;
        using BoostErrorCode = boost::system::error_code;

        /**
         * @brief Ctor.
         */
        [[deprecated]]
        TcpSocket() noexcept;

        /**
         * @brief Ctor.
         */
        [[deprecated]]
        TcpSocket(TcpSocket &) = delete;

        /**
         * @brief Dtor.
         */
        [[deprecated]]
        ~TcpSocket() noexcept override;

        /**
         * @brief
         *
         * @param message
         *
         * @return
         */
        [[deprecated]]
        [[nodiscard]]
        std::size_t send(const std::string &message) noexcept override;

        /**
         * @brief
         */
        [[deprecated]]
        void cleanErrors() noexcept;

        /**
         * @brief
         */
        [[deprecated]]
        [[nodiscard]]
        bool tryToConnect() noexcept override;

        /**
         * @brief
         */
        [[deprecated]]
        void closeConnection() noexcept override;

        /**
         * @brief
         *
         * @param   address
         *
         * @return  ping success
         */
        [[deprecated]]
        [[nodiscard]]
        static bool checkEndPoint(const std::string &address) noexcept;

    private:
        /**
         * Destination endpoint
         */
        EndPoint _destinationEndPoint;

        /**
         * Source endpoint
         */
        EndPoint _sourceEndPoint;

        /**
         * List of socket's errors
         */
        std::vector<BoostErrorCode> _errors;

        /**
         * Socket ref
         */
        AsioTcp::socket _socket;

        /**
         * Socket status
         */
        SocketStatus _status;

        /**
         * @brief
         */
        [[deprecated]]
        void closeSocket() noexcept;

    public:
#pragma region Accessors

        [[nodiscard]]
        const EndPoint &destinationEndPoint() const noexcept;

        [[nodiscard]]
        const EndPoint &sourceEndPoint() const noexcept;

        [[nodiscard]]
        std::string getLastError() const noexcept;

        [[nodiscard]]
        const std::vector<BoostErrorCode> &getErrors() const noexcept;

        [[nodiscard]]
        SocketStatus status() noexcept override;

#pragma endregion Accessors

#pragma region Mutators

        void destinationEndPoint(EndPoint endPoint) noexcept;

        void sourceEndPoint(EndPoint endPoint) noexcept;

#pragma endregion Mutators
    };
}
