#pragma once

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <memory>

#include "Network/EndPoint.hpp"
#include "Network/ISocket.hpp"

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
        [[nodiscard]] std::size_t Send(const std::string &message_) noexcept override;

        /**
         *
         */
        void CleanErrors() noexcept;

        /**
         *
         */
        [[nodiscard]] bool TryToConnect() noexcept override;

        /**
         *
         */
        void CloseConnection() noexcept override;

        /**
         *
         * @param address
         * @return          ping success
         */
        [[nodiscard]] static bool CheckEndPoint(const std::string &address) noexcept;

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
        void CloseSocket() noexcept;

    public:
#pragma region Accessors

        [[nodiscard]] const EndPoint &DestinationEndPoint() const noexcept;

        [[nodiscard]] const EndPoint &SourceEndPoint() const noexcept;

        [[nodiscard]] std::string GetLastError() const noexcept;

        [[nodiscard]] const std::vector<BoostErrorCode> &GetErrors() const noexcept;

        [[nodiscard]] SocketStatus Status() noexcept override;

#pragma endregion Accessors

#pragma region Mutators

        void DestinationEndPoint(EndPoint endPoint_) noexcept;

        void SourceEndPoint(EndPoint endPoint_) noexcept;

#pragma endregion Mutators
    };
}
