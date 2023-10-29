#include "WorLibrary/Sql/Event/EventManager.hpp"

using namespace Wor::Sql::Event;

EventManager::EventManager()
        : _empty(true),
          _eventList {} {
}

bool EventManager::Configure(EventList eventList) noexcept {
    if (eventList.empty()) {
        return false;
    }
    _eventList.resize(eventList.size());
    std::move(eventList.begin(), eventList.end(), _eventList.begin());
    _empty = false;
    return true;
}

void EventManager::UpdateEventList(bool executeNewEvents) noexcept {
    EventType y {
            {},
            0
    };
    auto updateEvent =
            std::find(std::begin(_eventList), std::end(_eventList),
                      EventType {{}, -1 });
    if (updateEvent == _eventList.end()) {
        return;
    }
    if (!executeNewEvents) {
        return;
    }
    auto newEvents = updateEvent->eventFunction(_lastEventId);
    _lastEventId += static_cast<std::int64_t>(newEvents.Size() - 1);
    ExecuteEvents(newEvents);
}

void EventManager::ExecuteEvents(const DbTableView &answerList) noexcept {
    for (const auto &cell : answerList) {
        auto f = cell.second;
        std::int16_t eventType = std::atoi(std::any_cast<std::string>(f[1].value).c_str());
        std::int64_t eventId = std::atoi(std::any_cast<std::string>(f[0].value).c_str());
        auto event =
                std::find(std::begin(_eventList), std::end(_eventList),
                          EventType {{}, eventType });
        auto ans = event->eventFunction(eventId);
    }
}

#pragma region Accessors

bool EventManager::IsEmpty() const noexcept {
    return _empty;
}

#pragma endregion Accessors

#pragma region Mutators

#pragma endregion Mutators
