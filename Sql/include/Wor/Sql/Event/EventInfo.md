
List of events `EventManager::eventList : std::vector<EventType>` store
all events to execute. It fills via `EventManager::Configure(EventList)` with eventList from Utils

To execute event use `EventManager::ExecuteEvents(const DbTableView&)`. This method calls from 
`EventManager::UpdateEventList(bool executeNewEvents)`, where we just updated DB state and store
all new events to `DbTableView`

