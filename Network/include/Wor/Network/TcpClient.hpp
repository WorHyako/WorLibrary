#pragma once

#include <memory>

#include "boost/asio.hpp"

namespace Wor::Network {
	/**
	 * @brief
	 *
	 * @usage
	 * @code
	 *
	 * @endcode
	 *
	 * @author WorHyako
	 */
	class TcpClient final {
	public:
		/**
		 * @brief	Ctor.
		 *
		 * @param	ctx
		 */
		explicit TcpClient(boost::asio::io_service& ctx) noexcept;

		/**
		 * @brief
		 *
		 * @param	endPoint
		 */
		void start(const boost::asio::ip::tcp::resolver::iterator& endPoint) noexcept;

		/**
		 * @brief
		 */
		void stop() noexcept;

		/**
		 * @brief
		 *
		 * @param	message
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
		 *
		 * @param	endPoint
		 */
		void handleConnect(const boost::asio::ip::tcp::resolver::iterator& endPoint) noexcept;

		/**
		 * @brief
		 */
		boost::asio::ip::tcp::socket _socket;

		/**
		 * @brief
		 */
		boost::asio::streambuf _readBuffer;

		/**
		 * @brief
		 */
		bool _stopped;

		/**
		 * @brief
		 */
		std::function<void(std::string)> _readCallback;

#pragma region Accessors/Methods;

	public:
		/**
		 * @brief
		 *
		 * @return	@code true @endcode
		 *			<p>
		 *			@code false @endcode
		 */
		[[nodiscard]]
		bool isRunning() const noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void readCallback(std::function<void(std::string)> callback) noexcept;

#pragma endregion Accessors/Methods;
	};
}
