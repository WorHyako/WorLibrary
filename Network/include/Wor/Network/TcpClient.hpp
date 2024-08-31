#pragma once

#include <memory>

#include "boost/asio.hpp"

namespace Wor::Network {

	/**
	 * @brief
	 *
	 * @usage
	 * @code
	 * @endcode
	 *
	 * @author WorHyako
	 */
	class TcpClient final {
	public:
		/**
		 * @brief Ctor.
		 */
		explicit TcpClient(boost::asio::io_service& ctx) noexcept;

		/**
		 * @brief
		 */
		void start(const boost::asio::ip::tcp::resolver::iterator& endPoint) noexcept;

		/**
		 * @brief
		 */
		void stop() noexcept;

		/**
		 * @brief
		 *
		 * @param message
		 */
		void send(const std::string& message) noexcept;

	private:
		/**
		 * @brief
		 */
		void startRead() noexcept;

		/**
		 * @brief
		 */
		void handleRead() noexcept;

		/**
		 * @brief
		 */
		void handleWrite() noexcept;

		/**
		 * @brief
		 */
		void handleConnect(const boost::asio::ip::tcp::resolver::iterator& endPoint) noexcept;

		boost::asio::ip::tcp::socket _socket;

		boost::asio::streambuf _readBuffer;

		bool _stopped;

		std::function<void(std::string)> _readCallback;

#pragma region Accessors/Methods;
	public:
		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		bool isRunning() const noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void readCallback(std::function<void(std::string)> callback = nullptr) noexcept;

#pragma endregion Accessors/Methods;
	};
}
