#pragma once

#include <vector>
#include <utility>
#include <string>
#include <thread>
#include <mutex>

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
         * @param eventList
         * @return
         */
        [[nodiscard]] bool configure(EventList eventList) noexcept;

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
        std::int64_t _lastEventId = 0;

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
