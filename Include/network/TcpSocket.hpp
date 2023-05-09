#ifndef WORLIBRARY_NETWORK_TCPSOCKET_HPP
#define WORLIBRARY_NETWORK_TCPSOCKET_HPP

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <memory>

#include "network/EndPoint.hpp"
#include "network/ISocket.hpp"

namespace wor::network {

    /**
     *
     */
    class TcpSocket : public ISocket {
    public:
        using AsioTcp = boost::asio::ip::tcp;
        using AsioTcpEndPoint = AsioTcp::endpoint;

        explicit TcpSocket(boost::asio::io_context &context) noexcept;

        ~TcpSocket() noexcept;

        std::size_t Send(const std::string &message_) noexcept override;

        /**
         *
         */
        void CleanErrors() noexcept;

        /**
         *
         */
        [[nodiscard]] bool TryToConnect() noexcept;

        /**
         *
         */
        void CloseConnection() noexcept;

        /**
         *
         * @param address
         * @return          ping success
         */
        [[nodiscard]] static bool CheckEndPoint(const std::string &address) noexcept;

    private:
        EndPoint _destinationEndPoint;

        EndPoint _sourceEndPoint;

        std::vector<boost::system::error_code> _errors;

        AsioTcp::socket _socket;

    public:
#pragma region Accessors

        [[nodiscard]] const EndPoint& DestinationEndPoint() const noexcept;

        [[nodiscard]] const EndPoint& SourceEndPoint() const noexcept;

        [[nodiscard]] std::string GetLastError() const noexcept;

        [[nodiscard]] const std::vector<boost::system::error_code> &
        GetErrors() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

        [[nodiscard]] EndPoint::Status DestinationEndPoint(EndPoint endPoint_) noexcept;

        [[nodiscard]] EndPoint::Status SourceEndPoint(EndPoint endPoint_) noexcept;

#pragma endregion Mutators
    };
}
#endif
