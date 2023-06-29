#ifndef WORLIBRARY_NETWORK_ISOCKET_HPP
#define WORLIBRARY_NETWORK_ISOCKET_HPP

#include <string>

namespace wor::Network {

    /**
     *
     */
    class ISocket {
    public:
        ISocket() = default;

        virtual ~ISocket() noexcept = default;

        /**
         *
         * @param message_
         */
        [[nodiscard]] virtual std::size_t Send(const std::string &message_) noexcept = 0;
    };
}
#endif
