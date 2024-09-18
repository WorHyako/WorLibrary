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
		 *
		 * @param	endpoint
		 *
		 * @return	@code true @endcode
		 *			<p>
		 *			@code false @endcode
		 */
		bool bindTo(const boost::asio::ip::udp::endpoint& endpoint) noexcept;

		/**
		 * @brief
		 */
		void start() noexcept;

		/**
		 * @brief
		 */
		void stop() noexcept;

		/**
		 * @brief
		 *
		 * @param	remoteEndpoint
		 */
		void parseBuffer(const boost::asio::ip::udp::endpoint& remoteEndpoint) noexcept;

	private:
		/**
		 * @brief
		 */
		void startReading() noexcept;

		/**
		 * @brief	Storage for receiving callbacks.
		 */
		std::unordered_map<boost::asio::ip::udp::endpoint,
						   std::function<void(const std::string&)>> _receiveCallbacks;

		/**
		 * @brief	Buffer for incoming packets.
		 */
		boost::asio::streambuf _buffer;

		/**
		 * @brief	Socket.
		 */
		std::unique_ptr<boost::asio::ip::udp::socket> _socket;

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
		bool isRunning() const noexcept;

		/**
		 * @brief
		 *
		 * @param remoteEndpoint
		 *
		 * @param callback
		 */
		void receiveCallback(boost::asio::ip::udp::endpoint remoteEndpoint,
							 std::function<void(const std::string&)> callback) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		boost::asio::ip::udp::endpoint endpoint() const noexcept;

#pragma endregion Accessors/Mutators
	};
}
