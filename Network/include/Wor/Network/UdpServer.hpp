#pragma once

#include <functional>
#include <memory>

#include <boost/asio.hpp>

namespace Wor::Network {
	/**
	 * @brief
	 *
	 * @usage
	 * @code
	 *
	 * @endcode
	 *
	 * @author	WorHyako
	 */
	class UdpServer {
	public:
		using Endpoint = boost::asio::ip::udp::endpoint;
		using Socket = boost::asio::ip::udp::socket;
		using Callback = std::function<void(const std::string&)>;
		/**
		 * @brief	Ctor.
		 */
		UdpServer() noexcept = default;

		/**
		 * @brief	Dtor.
		 */
		virtual ~UdpServer() noexcept;

		/**
		 * @brief
		 */
		void start(const Endpoint& endpoint) noexcept;

		/**
		 * @brief
		 */
		void stop() noexcept;

		/**
		 * @brief
		 *
		 * @param endpoint
		 *
		 * @param message
		 */
		void sendTo(const Endpoint& endpoint, const std::string& message) noexcept;

	private:
		/**
		 * @brief
		 *
		 * @param	remoteEndpoint
		 */
		void parseBuffer(const Endpoint& remoteEndpoint) noexcept;

		/**
		 * @brief
		 */
		void startReading() noexcept;

		/**
		 * @brief	Storage for receiving callbacks.
		 */
		std::unordered_map<Endpoint, Callback> _receiveCallbacks;

		/**
		 * @brief	Buffer for incoming packets.
		 */
		boost::asio::streambuf _buffer;

		/**
		 * @brief	Socket.
		 */
		std::unique_ptr<Socket> _socket;

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
		 * @param remoteEndpoint
		 *
		 * @param callback
		 */
		void receiveCallback(Endpoint remoteEndpoint, Callback callback) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		Endpoint endpoint() const noexcept;

#pragma endregion Accessors/Mutators
	};
}
