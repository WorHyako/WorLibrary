#include "WorLibrary/Sql/DbRowView.hpp"

using namespace Wor::Sql;

void DbRowView::Add(const DbCellView &element) noexcept {
    _rowView.emplace_back(element);
}

#pragma region Operators

DbCellView &DbRowView::operator[](std::int64_t idx) noexcept {
    return _rowView[idx];
}

std::vector<DbCellView>::iterator DbRowView::begin() noexcept {
    return std::begin(_rowView);
}

std::vector<DbCellView>::const_iterator DbRowView::begin() const noexcept {
    return std::begin(_rowView);
}

std::vector<DbCellView>::iterator DbRowView::end() noexcept {
    return std::end(_rowView);
}

std::vector<DbCellView>::const_iterator DbRowView::end() const noexcept {
    return std::end(_rowView);
}

std::vector<DbCellView>::const_iterator DbRowView::cbegin() const noexcept {
    return std::cbegin(_rowView);
}

std::vector<DbCellView>::const_iterator DbRowView::cend() const noexcept {
    return std::cend(_rowView);
}

#pragma endregion Operators
