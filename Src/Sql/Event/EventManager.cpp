#include "WorLibrary/Sql/Event/EventManager.hpp"

using namespace Wor::Sql::Event;

EventManager::EventManager() noexcept
        : _empty(true),
          _eventList {},
          _updatingActivity(false),
          _updatingTimeBreak(10) {
}

EventManager::~EventManager() noexcept {
    stopUpdatingThread();
}

void EventManager::startUpdatingThread() noexcept {
    _updatingThread = std::thread(&EventManager::updateEventList, this, false);
}

void EventManager::stopUpdatingThread() noexcept {
    _updatingActivity = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (_updatingThread.joinable()) {
        _updatingThread.join();
    }
}

bool EventManager::configure(EventList eventList) noexcept {
    if (eventList.empty()) {
        return false;
    }
    _eventList.resize(eventList.size());
    std::move(eventList.begin(), eventList.end(), _eventList.begin());
    _empty = false;
    return true;
}

void EventManager::updateEventListOnOtherThread(bool executeNewEvents) noexcept {
    while (_updatingActivity) {
        updateEventList();
        std::this_thread::sleep_for(std::chrono::milliseconds(_updatingTimeBreak));
    }
}

void EventManager::updateEventList(bool executeNewEvents) noexcept {
    const std::lock_guard<std::mutex> lock(_mutex);
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
    _lastEventId = std::atoi(newEvents.back().find("EventID").c_str());
    if (!executeNewEvents) {
        return;
    }
    executeEvents(newEvents);
}

void EventManager::executeEvents(const DbTableView &answerList) noexcept {
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

bool EventManager::isEmpty() const noexcept {
    return _empty;
}

#pragma endregion Accessors

#pragma region Mutators

#pragma endregion Mutators
