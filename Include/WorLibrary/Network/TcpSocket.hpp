#pragma once

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <memory>

#include "WorLibrary/Network/EndPoint.hpp"
#include "WorLibrary/Network/ISocket.hpp"

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
        TcpSocket() noexcept;

        /**
         * @brief Ctor.
         */
        TcpSocket(TcpSocket &) = delete;

        /**
         * @brief Dtor.
         */
        ~TcpSocket() noexcept override;

        /**
         * @brief
         *
         * @param message
         *
         * @return
         */
        [[nodiscard]]
        std::size_t send(const std::string &message) noexcept override;

        /**
         * @brief
         */
        void cleanErrors() noexcept;

        /**
         * @brief
         */
        [[nodiscard]]
        bool tryToConnect() noexcept override;

        /**
         * @brief
         */
        void closeConnection() noexcept override;

        /**
         * @brief
         *
         * @param   address
         *
         * @return  ping success
         */
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
