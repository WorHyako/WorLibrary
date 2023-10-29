#pragma once

#include <vector>
#include <utility>
#include <string>

#include "WorLibrary/Sql/Event/EventType.hpp"

namespace Wor::Sql::Event {

    /**
     *
     */
    class EventManager {
    public:
        /**
         * Ctor
         */
        EventManager();

        /**
         *
         * @param executeNewEvents
         */
        void UpdateEventList(bool executeNewEvents = true) noexcept;

        /**
         *
         */
        void ExecuteEvents(const DbTableView& answerList) noexcept;

        /**
         *
         * @param eventList
         * @return
         */
        [[nodiscard]] bool Configure(EventList eventList) noexcept;

    public:
        /**
         *
         */
        EventList _eventList;

        /**
         *
         */
        std::int64_t _lastEventId = 0;

        /**
         *
         */
        bool _empty;

    public:
#pragma region Accessors

        [[nodiscard]] inline bool IsEmpty() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

#pragma endregion Mutators
    };
}
