#include "Network/Utils/IoService.hpp"

#include <memory>
#include <thread>

#include "boost/asio.hpp"

using namespace Wor::Network::Utils;
using namespace boost::asio;

namespace {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	pthread_t io_thread_handle;

	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	io_context ctx;
}

[[nodiscard]]
io_context& IoService::get() noexcept {
	return ::ctx;
}

void IoService::run() noexcept {
	if (isRunning()) {
		return;
	}
	std::thread t([&io{::ctx}]() {
		std::ignore = io.run();
	});
	::io_thread_handle = t.native_handle();
	t.detach();
}

bool IoService::isRunning() noexcept {
	return ::io_thread_handle != nullptr;
}
