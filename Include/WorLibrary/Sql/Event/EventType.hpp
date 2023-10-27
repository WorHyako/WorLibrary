#pragma once

#include <functional>

#include "WorLibrary/Sql/DbCellStrView.hpp"

namespace Wor::Sql::Event {

    /**
     *
     */
    struct EventType {

        /**
         *
         */
        std::function<TableMap<std::string>(std::uint64_t)> eventFunction;

        /**
         *
         */
        std::int32_t eventId;
    };

    /**
     *
     */
    using EventList = std::vector<EventType>;
}
