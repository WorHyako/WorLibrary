#pragma once

#include <boost/asio.hpp>

#include <functional>

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
	 *
	 * @see		@code TcpServer @endcode
	 */
	class TcpSession final
			: public std::enable_shared_from_this<TcpSession>,
			  public boost::asio::noncopyable {
	public:
		using ptr = std::shared_ptr<TcpSession>;

		/**
		 * @brief
		 */
		void run() noexcept;

		/**
		 * @brief
		 *
		 * @param ioService
		 *
		 * @return
		 */
		[[nodiscard]]
		static TcpSession::ptr create(boost::asio::io_service& ioService) noexcept;

		/**
		 * @brief
		 */
		void close() noexcept;

		/**
		 * @brief
		 *
		 * @param	message
		 */
		void send(const std::string& message) noexcept;

	private:
		/**
		 * @brief	Ctor.
		 *
		 * @param	ioContext
		 */
		explicit TcpSession(boost::asio::io_service& ioContext) noexcept;

		/**
		 * @brief
		 */
		void startReading() noexcept;

		/**
		 * @brief
		 */
		void parseBuffer() noexcept;

		boost::asio::ip::tcp::socket _socket;

		boost::asio::streambuf _buffer;

		std::string _name;

		std::function<void(TcpSession::ptr)> _closeCallback;

		std::function<void(const std::string& message)> _receiveCallback;

		bool _isActive;

	public:
#pragma region Accessors/Mutators

		/**
		 * @brief	Socket accessor.
		 *
		 * @return	Socket reference.
		 */
		[[nodiscard]]
		boost::asio::ip::tcp::socket& socket() noexcept;

		/**
		 * @brief
		 *
		 * @param name
		 */
		void name(std::string name) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		const std::string& name() noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		boost::asio::ip::tcp::endpoint endpoint() const noexcept;

		/**
		 * @brief
		 *
		 * @param callback
		 */
		void closeCallback(std::function<void(TcpSession::ptr)> callback) noexcept;

		/**
		 * @brief	Receive callback accessor.
		 *			<p>
		 *			It will be called on receive message event from remote device.
		 *
		 * @param	callback Receive callback.
		 */
		void receiveCallback(std::function<void(const std::string& message)> callback) noexcept;

#pragma endregion Accessors/Mutators
	};
}
