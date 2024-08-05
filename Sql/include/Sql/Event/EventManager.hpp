#pragma once

#include <thread>
#include <mutex>

#include "Sql/Event/EventType.hpp"

namespace Wor::Sql::Event {

    /**
     *
     */
    class EventManager {
    public:
        /**
         * Ctor
         */
        EventManager() noexcept;

        /**
         * Dtor
         */
        ~EventManager() noexcept;

        /**
         *
         */
        void executeEvents(const DbTableView& answerList) noexcept;

        /**
         *
         * @param updateType
         */
        void setUpdateEvent(UpdateEventType updateType) noexcept;

        /**
         *
         * @param eventList
         */
        void setEventList(EventList eventList) noexcept;

        /**
         *
         */
         void startUpdatingThread() noexcept;

         /**
          *
          */
         void stopUpdatingThread() noexcept;

    private:
        /**
         *
         */
         std::thread _updatingThread;

        /**
         *
         */
        EventList _eventList;

        /**
         *
         */
        UpdateEventType _updateEvent;

        /**
         *
         */
        std::int64_t _lastEventId;

        /**
         *
         */
        bool _empty;

        /**
         *
         */
        bool _updatingActivity;

        /**
         *
         */
        std::chrono::milliseconds _updatingTimeBreak;

        /**
         *
         */
        std::mutex _mutex;

        /**
         *
         * @param executeNewEvents
         */
        void updateEventList(bool executeNewEvents = true) noexcept;

        /**
         *
         */
        void updateEventListOnOtherThread(bool executeNewEvents = true) noexcept;

    public:
#pragma region Accessors

        [[nodiscard]] inline bool isEmpty() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

#pragma endregion Mutators
    };
}
