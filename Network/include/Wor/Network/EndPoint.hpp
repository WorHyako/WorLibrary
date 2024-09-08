#pragma once

#include <string>
#include <cstdint>

namespace Wor::Network {

    /**
     * @brief
     *
     * @author WorHyako
     */
    struct EndPoint {
        /**
         * @brief
         *
         * @author WorHyako
         */
        enum class Status :
                std::uint8_t {
            ZeroCheck,
            Unreachable,
            Ready,
            Error
        };

        /**
         * @brief Ctor.
         *
         * @param address
         *
         * @param port
         */
        explicit EndPoint(std::string address = {}, std::uint32_t port = 0) noexcept;

        std::string _address;

        std::uint32_t _port;

        Status _status;
    };
}
