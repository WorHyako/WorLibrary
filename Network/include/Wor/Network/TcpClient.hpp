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
		using Callback = std::function<void(const std::string_view&)>;
		using IoContext = boost::asio::io_context;
		using Socket = boost::asio::ip::tcp::socket;
		using Endpoint = boost::asio::ip::tcp::endpoint;

		/**
		 * @brief	Ctor.
		 *
		 * @param	ctx
		 */
		explicit TcpClient(IoContext& ctx) noexcept;

		/**
		 * @brief
		 *
		 * @param	endpoint
		 */
		void start(const Endpoint& endpoint) noexcept;

		/**
		 * @brief
		 */
		void stop() noexcept;

		/**
		 * @brief
		 *
		 * @param	message
		 */
		void send(const std::string_view& message) noexcept;

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
		 * @param	endpoint
		 */
		void handleConnect(const Endpoint& endpoint) noexcept;

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
		Callback _receive_callback;

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
