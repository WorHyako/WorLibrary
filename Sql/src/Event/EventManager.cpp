#include "Sql/Event/EventManager.hpp"

#include <algorithm>

using namespace Wor::Sql::Event;

EventManager::EventManager() noexcept
	: _lastEventId(0),
	  _empty(true),
	  _updatingActivity(false),
	  _updatingTimeBreak(10) {
}

EventManager::~EventManager() noexcept {
	stopUpdatingThread();
}

void EventManager::startUpdatingThread() noexcept {
	_updatingThread = std::thread(&EventManager::updateEventList, this, true);
}

void EventManager::stopUpdatingThread() noexcept {
	_updatingActivity = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	if (_updatingThread.joinable()) {
		_updatingThread.join();
	}
}

void EventManager::setUpdateEvent(UpdateEventType updateType) noexcept {
	_updateEvent = std::move(updateType);
}

void EventManager::setEventList(EventList eventList) noexcept {
	if (eventList.empty()) {
		return;
	}
	_eventList.resize(eventList.size());
	std::move(std::begin(eventList), std::end(eventList), std::begin(_eventList));
	_empty = false;
}

void EventManager::updateEventListOnOtherThread(bool executeNewEvents) noexcept {
	while (_updatingActivity) {
		updateEventList();
		std::this_thread::sleep_for(std::chrono::milliseconds(_updatingTimeBreak));
	}
}

void EventManager::updateEventList(bool executeNewEvents) noexcept {
	auto newEvents = _updateEvent.eventFunction(0);
	newEvents.sort();
	_lastEventId = std::stoi(newEvents.back().find("EventID"));
	if (!executeNewEvents) {
		return;
	}
	executeEvents(newEvents);
}

void EventManager::executeEvents(const DbTableView &answerList) noexcept {
	for (const auto &row : answerList) {
		const std::int64_t eventId = std::stol(row[0]._value);
		const std::int32_t eventType = std::stoi(row[1]._value);
		const auto event =
				std::find(std::begin(_eventList),
						  std::end(_eventList),
						  EventType{
								  .eventFunction = {},
								  .eventId = eventType
						  });
		if (event == std::end(_eventList)) {
			continue;
		}
		event->eventFunction(eventId);
	}
}

#pragma region Accessors/Mutators

bool EventManager::isEmpty() const noexcept {
	return _empty;
}

#pragma endregion Accessors/Mutators
