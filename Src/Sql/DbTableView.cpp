#include "WorLibrary/Sql/DbTableView.hpp"

#include <algorithm>

using namespace Wor::Sql;

std::vector<DbRowView>::iterator DbTableView::Find(const DbRowView &element) const noexcept {
    return {};
}

std::vector<DbRowView>::iterator DbTableView::Find(std::int64_t eventId) const noexcept {
    return {};
}

void DbTableView::Sort(const std::string& property) noexcept {
    std::sort(std::begin(_tableView), std::end(_tableView),
              [&property](const DbRowView &a, const DbRowView &b) {
                  auto aEventId = std::atoi(a.Find(property).c_str());
                  auto bEventId = std::atoi(b.Find(property).c_str());
                  return aEventId < bEventId;
              });
}

#pragma region Accessors

std::optional<DbRowView> DbTableView::GetRow(std::int64_t eventId) const noexcept {
    auto event = Find(eventId);
    return event != _tableView.end()
           ? *event
           : std::optional<DbRowView>();
}

std::size_t DbTableView::Size() const noexcept {
    return _tableView.size();
}

#pragma endregion Accessors

#pragma region Mutators

void DbTableView::AddRow(DbRowView row, bool overwrite) noexcept {
    const auto rowExist = Find(row) != _tableView.end();
    if ((rowExist && overwrite) || !rowExist) {
        _tableView.emplace_back(std::move(row));
    }
}

#pragma endregion Mutators

#pragma region Operators

DbRowView &DbTableView::operator[](std::int64_t idx) noexcept {
    return _tableView[idx];
}

const DbRowView &DbTableView::operator[](std::int64_t idx) const noexcept {
    return _tableView[idx];
}

std::vector<DbRowView>::iterator DbTableView::begin() noexcept {
    return std::begin(_tableView);
}

std::vector<DbRowView>::const_iterator DbTableView::begin() const noexcept {
    return std::begin(_tableView);
}

std::vector<DbRowView>::iterator DbTableView::end() noexcept {
    return std::end(_tableView);
}

std::vector<DbRowView>::const_iterator DbTableView::end() const noexcept {
    return std::end(_tableView);
}

std::vector<DbRowView>::const_iterator DbTableView::cbegin() const noexcept {
    return std::cbegin(_tableView);
}

std::vector<DbRowView>::const_iterator DbTableView::cend() const noexcept {
    return std::cend(_tableView);
}

#pragma endregion Operators
