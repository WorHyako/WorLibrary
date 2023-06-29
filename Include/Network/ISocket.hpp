#ifndef WORLIBRARY_NETWORK_ISOCKET_HPP
#define WORLIBRARY_NETWORK_ISOCKET_HPP

#include <string>

namespace wor::network {

    /**
     *
     */
    class ISocket {
    public:
        ISocket() = default;

        /**
         *
         * @param message_
         */
        virtual std::size_t Send(const std::string &message_) noexcept = 0;
    };
}
#endif
