#pragma once

#include <memory>

#include "boost/asio.hpp"

#include "Wor/Network/TcpSession.hpp"

namespace Wor::Network {
	/**
	 * @brief
	 *			<p>
	 *			Logs all operations.
	 *			<p>
	 *			Architect to use under @code Singleton @endcode wrapper.
	 *
	 * @usage
	 * @code
	 *
	 * @endcode
	 *
	 * @author	WorHyako
	 *
	 * @see		@code TcpSession @endcode
	 */
	class TcpServer {
	public:
		using Endpoint = boost::asio::ip::tcp::endpoint;
		using Acceptor = boost::asio::ip::tcp::acceptor;
		using Callback = std::function<void(const std::string_view&)>;

		/**
		 * @brief	Ctor.
		 *			<p>
		 *			Object has default constructor to allow initialization through @code Singleton @endcode.
		 */
		TcpServer() noexcept = default;

		/**
		 * @brief	Dtor.
		 */
		virtual ~TcpServer() noexcept;

		/**
		 * @brief	Tries to bind server to local endpoint and start to listen this one.
		 *			<p>
		 *			Use reuse address option by default.
		 *			<p>
		 *			If server is running already, tries restart it.
		 *
		 * @param	endpoint Local endpoint.
		 *
		 * @return	@code true @endcode		Port has been linked to local endpoint.
		 *			<p>
		 *			@code false @endcode	Error in port linking.
		 */
		[[nodiscard]]
		bool bindTo(const Endpoint& endpoint) noexcept;

		/**
		 * @brief	Listens local port for a new session to make new sessions.
		 *			<p>
		 *			Requires successful binding to local port via @code TcpServer::bindTo @endcode method.
		 */
		void start() noexcept;

		/**
		 * @brief	Stops server, reset acceptor, closes all current sessions, clears session list.
		 *			<p>
		 *			Nothing happened if server is not running.
		 */
		void stop() noexcept;

		/**
		 * @brief	Closes session with selected name.
		 *
		 * @param	name Session's name.
		 */
		void closeSession(const std::string_view& name) noexcept;

		/**
		 * @brief	Closes session with selected endpoint.
		 *
		 * @param	remote_endpoint Session's endpoint.
		 */
		void closeSession(const Endpoint& remote_endpoint) noexcept;

		/**
		 * @brief	Sends selected message to all available endpoints.
		 *
		 * @param	message	Message.
		 */
		void sendToAll(const std::string_view& message) const noexcept;

	private:
		/**
		 * @brief	Closes selected session.
		 *       	<p/>
		 *       	Nothing happened, If session doesn't exist in _sessionList.
		 *
		 * @param	session	Session ptr to close.
		 */
		void closeSession(const TcpSession::ptr& session) noexcept;

		/**
		 * @brief	Callback for accept handling.
		 *			Puts session pointer to session storage and applies receiving callback for session if available.
		 *			<p>
		 *			Recursively start listening port for new session.
		 *			<p>
		 *			Method's signature responds boost callback signature.
		 *
		 * @param	sessionPtr	Session pointer
		 *
		 * @param	ec			Error code.
		 */
		void handleAccept(const TcpSession::ptr& sessionPtr, boost::system::error_code ec) noexcept;

		/**
		 * @brief	Boost acceptor to listen local port and accept new connections.
		 */
		std::unique_ptr<Acceptor> _acceptor;

		/**
		 * @brief	Storage for sessions pointers.
		 *			<p>
		 *			It is replenished with session accepting.
		 */
		std::vector<TcpSession::ptr> _sessions_list;

		/**
		 * @brief	Storage for receive callbacks.
		 */
		std::unordered_map<Endpoint, Callback> _receive_callbacks;

	public:
#pragma region Accessors/Mutators

		/**
		 * @brief	Session pointer accessor.
		 *
		 * @param	alias Session's name.
		 *
		* @return	Session pointer.
		 *			<p>
		 *			@code nullptr @endcode If session doesn't exist.
		 */
		[[nodiscard]]
		TcpSession::ptr session(const std::string_view& alias) noexcept;

		/**
		 * @brief	Session pointer accessor.
		 *
		 * @param	endpoint Session's remote endpoint.
		 *
		 * @return	Session pointer.
		 *			<p>
		 *			@code nullptr @endcode If session doesn't exist.
		 */
		[[nodiscard]]
		TcpSession::ptr session(const Endpoint& endpoint) noexcept;

		/**
		 * @brief	Session storage accessor.
		 *
		 * @return	Current session list.
		 */
		[[nodiscard]]
		std::vector<TcpSession::ptr> sessionList() const noexcept;

		/**
		 * @brief	Check acceptor is running.
		 *			It's means server has been linked to local port and listen.
		 *
		 * @return	@code true @endcode		Server listens port.
		 *			<p>
		 *			@code false @endcode	Server listens port.
		 */
		[[nodiscard]]
		bool bound() const noexcept;

		/**
		 * @brief	Receive callback accessor for selected session.
		 *			<p>
		 *			Pair of callback and session's endpoint will be storage to @code receiveCallback @endcode field.
		 *
		 * @param	endpoint Session's endpoint.
		 *
		 * @param	callback Callback.
		 */
		void receiveCallback(Endpoint endpoint, Callback callback) noexcept;

#pragma endregion Accessors/Mutators
	};
}
