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
         * @param eventList
         * @return
         */
        [[nodiscard]] bool Configure(EventList eventList) noexcept;

    private:
    public:
        /**
         *
         */
        EventList _eventList;

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
