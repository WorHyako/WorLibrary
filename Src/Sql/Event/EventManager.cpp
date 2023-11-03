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
    auto updateEvent =
            std::find_if(std::begin(_eventList), std::end(_eventList),
                         [](const EventType &a) {
                             return a.eventId == -1;
                         });
    if (updateEvent == _eventList.end()) {
        return;
    }
    auto newEvents = updateEvent->eventFunction(0);
    newEvents.Sort();
    if (!executeNewEvents) {
        return;
    }
    ExecuteEvents(newEvents);
}

void EventManager::ExecuteEvents(const DbTableView &answerList) noexcept {
    for (const auto &row : answerList) {
        const std::int64_t eventId = std::stol(row[0].value);
        const std::int32_t eventType = std::stoi(row[1].value);
        const auto event =
                std::find(std::begin(_eventList), std::end(_eventList),
                          EventType {{}, eventType });
        if (event == _eventList.end()) {
            continue;
        }
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
