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
		using Callback = std::function<void(const std::string&)>;
		using Socket = boost::asio::ip::tcp::socket;
		using Endpoint = boost::asio::ip::tcp::endpoint;

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
		void parseBuffer() noexcept;

		/**
		 * @brief
		 *
		 * @param	endPoint
		 */
		void handleConnect(const boost::asio::ip::tcp::resolver::iterator& endPoint) noexcept;

		/**
		 * @brief
		 */
		Socket _socket;

		/**
		 * @brief
		 */
		boost::asio::streambuf _buffer;

		/**
		 * @brief
		 */
		Callback _receiveCallback;

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
		bool bound() const noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void receiveCallback(Callback callback) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		Endpoint remoteEndpoint() const noexcept;

#pragma endregion Accessors/Methods;
	};
}
