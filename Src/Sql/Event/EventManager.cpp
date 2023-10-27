#include "WorLibrary/Sql/Event/EventManager.hpp"

using namespace Wor::Sql::Event;

EventManager::EventManager()
        : _empty(true),
          _eventList{} {
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

#pragma region Accessors

bool EventManager::IsEmpty() const noexcept {
    return _empty;
}

#pragma endregion Accessors

#pragma region Mutators

#pragma endregion Mutators
