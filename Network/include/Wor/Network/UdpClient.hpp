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
		/**
		 * @brief	Ctor.
		 */
		explicit UdpClient() noexcept = default;

		/**
		 * @brief
		 *
		 * @param	endpoint
		 */
		void start(const boost::asio::ip::udp::endpoint& endpoint) noexcept;

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
		 */
		boost::asio::ip::udp::endpoint _remoteEndpoint;

		/**
		 * @brief
		 */
		std::unique_ptr<boost::asio::ip::udp::socket> _socket;

		/**
		 * @brief
		 */
		boost::asio::streambuf _buffer;

		/**
		 * @brief
		 */
		std::function<void(std::string)> _readCallback;

	public:
#pragma region Accessors/Mutators

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
		 * @return
		 */
		[[nodiscard]]
		boost::asio::ip::udp::endpoint localEndpoint() const noexcept;

		/**
		 * @brief
		 *
		 * @param remoteEndpoint
		 */
		void remoteEndpoint(boost::asio::ip::udp::endpoint remoteEndpoint) noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void readCallback(std::function<void(std::string)> callback) noexcept;

#pragma endregion Accessors/Mutators
	};
}
