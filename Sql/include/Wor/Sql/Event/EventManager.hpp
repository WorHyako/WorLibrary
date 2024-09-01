#pragma once

#include <thread>
#include <mutex>

#include "Sql/Event/EventType.hpp"

namespace Wor::Sql::Event {
	/**
	 * @brief
	 *
	 * @author WorHYako
	 */
	class EventManager {
	public:
		/**
		 * @brief Ctor.
		 */
		EventManager() noexcept;

		/**
		 * @brief Dtor.
		 */
		~EventManager() noexcept;

		/**
		 * @brief
		 *
		 * @param answerList
		 */
		void executeEvents(const DbTableView& answerList) noexcept;

		/**
		 * @brief
		 *
		 * @param updateType
		 */
		void setUpdateEvent(UpdateEventType updateType) noexcept;

		/**
		 * @brief
		 *
		 * @param eventList
		 */
		void setEventList(EventList eventList) noexcept;

		/**
		 * @brief
		 */
		void startUpdatingThread() noexcept;

		/**
		 * @brief
		 */
		void stopUpdatingThread() noexcept;

	private:
		/**
		 *
		 */
		std::thread _updatingThread;

		/**
		 *
		 */
		EventList _eventList;

		/**
		 *
		 */
		UpdateEventType _updateEvent;

		/**
		 *
		 */
		std::int64_t _lastEventId;

		/**
		 *
		 */
		bool _empty;

		/**
		 *
		 */
		bool _updatingActivity;

		/**
		 *
		 */
		std::chrono::milliseconds _updatingTimeBreak;

		/**
		 * @brief
		 *
		 * @param executeNewEvents
		 */
		void updateEventList(bool executeNewEvents = true) noexcept;

		/**
		 * @brief
		 *
		 * @param executeNewEvents
		 */
		void updateEventListOnOtherThread(bool executeNewEvents = true) noexcept;

	public:
#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		inline bool isEmpty() const noexcept;

#pragma endregion Accessors/Mutators
	};
}
