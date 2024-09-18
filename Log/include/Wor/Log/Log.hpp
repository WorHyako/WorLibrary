#pragma once

#include <spdlog/spdlog.h>

#ifdef WOR_DEV_LOG
#define worInfo(...) SPDLOG_INFO(__VA_ARGS__)
#define worTrace(...) SPDLOG_TRACE(__VA_ARGS__)
#define worWarn(...) SPDLOG_WARN(__VA_ARGS__)
#define worError(...) SPDLOG_ERROR(__VA_ARGS__)
#else
#define worInfo(...) spdlog::info(__VA_ARGS__)
#define worTrace(...) spdlog::trace(__VA_ARGS__)
#define worWarn(...) spdlog::warn(__VA_ARGS__)
#define worError(...) spdlog::error(__VA_ARGS__)
#endif

namespace Wor::Log {
	/**
	 * @brief
	 *
	 * @author	WorHyako
	 */
	static void configureLogger() {
#ifdef WOR_DEV_LOG
		spdlog::set_level(spdlog::level::trace);
		spdlog::set_pattern("[%H:%M:%e] [%^%l%$] [thread %t] [source %s:%#] [function %!] \n%v");
#else
		spdlog::set_pattern("[%H:%M:%e] [%^%l%$] \n%v");
#endif
	}
}
