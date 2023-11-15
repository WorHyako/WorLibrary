#pragma once

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <memory>

#include "WorLibrary/Network/EndPoint.hpp"
#include "WorLibrary/Network/ISocket.hpp"

namespace Wor::Network {

    /**
     *
     */
    class TcpSocket : public ISocket {
    public:
        using AsioTcp = boost::asio::ip::tcp;
        using AsioTcpEndPoint = AsioTcp::endpoint;
        using BoostErrorCode = boost::system::error_code;

        /**
         * Ctor
         */
        TcpSocket() noexcept;

        /**
         * Ctor
         */
        TcpSocket(TcpSocket &) = delete;

        /**
         * Dtor
         */
        ~TcpSocket() noexcept override;

        /**
         *
         * @param message_
         * @return
         */
        [[nodiscard]] std::size_t send(const std::string &message_) noexcept override;

        /**
         *
         */
        void cleanErrors() noexcept;

        /**
         *
         */
        [[nodiscard]] bool tryToConnect() noexcept override;

        /**
         *
         */
        void closeConnection() noexcept override;

        /**
         *
         * @param address
         * @return          ping success
         */
        [[nodiscard]] static bool checkEndPoint(const std::string &address) noexcept;

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
         *
         */
        void closeSocket() noexcept;

    public:
#pragma region Accessors

        [[nodiscard]] const EndPoint &destinationEndPoint() const noexcept;

        [[nodiscard]] const EndPoint &sourceEndPoint() const noexcept;

        [[nodiscard]] std::string getLastError() const noexcept;

        [[nodiscard]] const std::vector<BoostErrorCode> &getErrors() const noexcept;

        [[nodiscard]] SocketStatus status() noexcept override;

#pragma endregion Accessors

#pragma region Mutators

        void destinationEndPoint(EndPoint endPoint_) noexcept;

        void sourceEndPoint(EndPoint endPoint_) noexcept;

#pragma endregion Mutators
    };
}
