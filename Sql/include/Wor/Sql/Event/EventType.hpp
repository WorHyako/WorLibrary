#pragma once

#include <functional>

#include "Sql/DbTableView.hpp"

namespace Wor::Sql::Event {
	/**
	 *
	 */
	struct EventType {
		friend bool operator==(const EventType& lhs, const EventType& rhs) {
			return lhs.eventId == rhs.eventId;
		}

		friend bool operator!=(const EventType& lhs, const EventType& rhs) {
			return !(rhs == lhs);
		}

		/**
		 *
		 */
		std::function<void(std::uint64_t)> eventFunction;

		/**
		 *
		 */
		std::int32_t eventId = 0;
	};

	/**
	 *
	 */
	using EventList = std::vector<EventType>;

	/**
	 *
	 */
	struct UpdateEventType {
		/**
		 *
		 */
		std::function<DbTableView(std::uint64_t)> eventFunction;

		/**
		 *
		 */
		std::int32_t eventId = 0;
	};
}
