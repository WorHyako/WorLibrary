#ifndef WORLIBRARY_NETWORK_ENDPOINT_HPP
#define WORLIBRARY_NETWORK_ENDPOINT_HPP

#include <string>

namespace wor::network {

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

        explicit EndPoint(std::string address_ = {}, int port_ = -1) noexcept;

        std::string address;

        unsigned short port;
    };
}
#endif
