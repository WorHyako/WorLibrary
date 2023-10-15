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
            ZeroCheck,
            Unreachable,
            Ready,
            Error
        } status;

        explicit EndPoint(std::string address_ = {}, uint32_t port_ = 0) noexcept;

        std::string address;

        uint32_t port;
    };
}
