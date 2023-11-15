#pragma once

#include <string>

namespace Wor::Network {

    /**
     *
     */
    struct EndPoint {
    public:

        /**
         *
         */
        enum class Status {
            /**
             * End point did'n
             */
            ZeroCheck,
            Unreachable,
            Ready,
            Error
        } status;

        explicit EndPoint(std::string address_ = {}, std::uint32_t port_ = 0) noexcept;

        std::string address;

        std::uint32_t port;
    };
}
