#pragma once

/**
 * Get symbol via string
 */
#define Name_ToS_tr(name) #name

/**
 * Generate EventType by Event Name
 */
#define Generate_Event_Type(EventName) {                    \
        [](std::int64_t eventId) {                          \
            return EventName(eventId);                      \
        },                                                  \
        static_cast<std::int32_t>(EventTypes::EventName )   \
    }
