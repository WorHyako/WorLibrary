#include "WorLibrary/Sql/DbTableView.hpp"

#include <algorithm>

using namespace Wor::Sql;

#pragma region Accessors

std::optional<DbRowView> DbTableView::GetRow(std::int64_t eventId) const noexcept {
    auto event = _tableView.find(eventId);
    return event != _tableView.end()
           ? event->second
           : std::optional<DbRowView>();
}

std::size_t DbTableView::Size() const noexcept {
    return _tableView.size();
}

#pragma endregion Accessors

#pragma region Mutators

bool DbTableView::AddRow(DbRowView row, bool overwrite) noexcept {
    const auto it = std::find_if(row.begin(), row.end(), [](DbCellView &each) {
        return each.name == "EventID";
    });
    if (it == row.end()) {
        return false;
    }
    const int eventId = std::atoi(std::any_cast<std::string>(it->value).c_str());
    const auto rowExist = _tableView.find(eventId) != _tableView.end();
    if ((rowExist && overwrite) || !rowExist) {
        _tableView[eventId] = std::move(row);
    }
    return true;
}

#pragma endregion Mutators

#pragma region Operators

DbRowView &DbTableView::operator[](std::int64_t idx) noexcept {
    return _tableView[idx];
}

std::map<std::int64_t, DbRowView>::iterator DbTableView::begin() noexcept {
    return std::begin(_tableView);
}

std::map<std::int64_t, DbRowView>::const_iterator DbTableView::begin() const noexcept {
    return std::begin(_tableView);
}

std::map<std::int64_t, DbRowView>::iterator DbTableView::end() noexcept {
    return std::end(_tableView);
}

std::map<std::int64_t, DbRowView>::const_iterator DbTableView::end() const noexcept {
    return std::end(_tableView);
}

std::map<std::int64_t, DbRowView>::const_iterator DbTableView::cbegin() const noexcept {
    return std::cbegin(_tableView);
}

std::map<std::int64_t, DbRowView>::const_iterator DbTableView::cend() const noexcept {
    return std::cend(_tableView);
}

#pragma endregion Operators
