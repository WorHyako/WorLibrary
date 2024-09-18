#include "Network/Utils/IoService.hpp"

#include <memory>
#include <thread>
#include <cstdio>

#include "boost/asio.hpp"

using namespace Wor::Network::Utils;
using namespace boost::asio;

namespace {

    /**
     * @brief
     *
     * @author WorHyako
     */
    pthread_t ioThreadHandle;

    /**
     * @brief
     *
     * @author WorHyako
     */
    io_service ioService;
}

[[nodiscard]]
io_service &IoService::get() noexcept {
    return ::ioService;
}

void IoService::run() noexcept {
    if (isRunning()) {
        return;
    }
    std::thread t([&io = ::ioService]() {
        boost::system::error_code ec;
        std::ignore = io.run(ec);
    });
    ::ioThreadHandle = t.native_handle();
    t.detach();
}

bool IoService::isRunning() noexcept {
    return ::ioThreadHandle != nullptr;
}
