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
		using Socket = boost::asio::ip::tcp::socket;
		using Endpoint = boost::asio::ip::tcp::endpoint;
		using IoContext = boost::asio::io_context;
		using Callback = std::function<void(const std::string_view&)>;

		/**
		 * @brief
		 */
		void run() noexcept;

		/**
		 * @brief
		 *
		 * @param ctx
		 *
		 * @return
		 */
		[[nodiscard]]
		static TcpSession::ptr create(IoContext& ctx) noexcept;

		/**
		 * @brief
		 */
		void close() noexcept;

		/**
		 * @brief
		 *
		 * @param	message
		 */
		void send(const std::string_view& message) noexcept;

	private:
		/**
		 * @brief	Ctor.
		 *
		 * @param	ctx
		 */
		explicit TcpSession(IoContext& ctx) noexcept;

		/**
		 * @brief
		 */
		void startReading() noexcept;

		/**
		 * @brief
		 */
		void parseBuffer() noexcept;

		Socket _socket;

		boost::asio::streambuf _buffer;

		std::string _alias;

		std::function<void(TcpSession::ptr)> _close_callback;

		Callback _receiveCallback;

	public:
#pragma region Accessors/Mutators

		/**
		 * @brief	Check session is running.
		 *			It's means session has been linked with remote endpoint.
		 *
		 * @return	@code true @endcode		Session is active.
		 *			<p>
		 *			@code false @endcode	Session is not active.
		 */
		[[nodiscard]]
		bool bound() const noexcept;

		/**
		 * @brief	Socket accessor.
		 *
		 * @return	Socket reference.
		 */
		[[nodiscard]]
		Socket& socket() noexcept;

		/**
		 * @brief
		 *
		 * @param alias
		 */
		void alias(const std::string_view& alias) noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		const std::string& alias() noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		Endpoint endpoint() const noexcept;

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
		void receiveCallback(Callback callback) noexcept;

#pragma endregion Accessors/Mutators
	};
}
