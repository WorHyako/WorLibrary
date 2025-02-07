#pragma once

#include <boost/asio.hpp>

#include <memory>

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
	class UdpClient {
	public:
		using Socket = boost::asio::ip::udp::socket;
		using Endpoint = boost::asio::ip::udp::endpoint;
		using Callback = std::function<void(const std::string_view&)>;
		/**
		 * @brief	Ctor.
		 */
		explicit UdpClient() noexcept = default;

		/**
		 * @brief	Dtor.
		 */
		virtual ~UdpClient() noexcept;

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
		 */
		Endpoint _remote_endpoint;

		/**
		 * @brief
		 */
		std::unique_ptr<Socket> _socket;

		/**
		 * @brief
		 */
		boost::asio::streambuf _buffer;

		/**
		 * @brief
		 */
		Callback _read_callback;

	public:
#pragma region Accessors/Mutators

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
		 * @return
		 */
		[[nodiscard]]
		Endpoint localEndpoint() const noexcept;

		/**
		 * @brief
		 *
		 * @param remote_endpoint
		 */
		void remoteEndpoint(Endpoint remote_endpoint) noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void readCallback(Callback callback) noexcept;

#pragma endregion Accessors/Mutators
	};
}
