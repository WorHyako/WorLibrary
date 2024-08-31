#include "Sql/DbRowView.hpp"

#include <algorithm>

using namespace Wor::Sql;

void DbRowView::add(DbCellView cell) noexcept {
	_rowView.emplace_back(std::move(cell));
}

std::string DbRowView::find(const std::string &cellName) const noexcept {
	auto element = std::ranges::find_if(_rowView,
										[&cellName](const DbCellView &cell) {
											return cellName == cell._name;
										});
	if (element == std::end(_rowView)) {
		return "";
	}
	return element->_value;
}

#pragma region Operators

DbCellView &DbRowView::operator[](std::size_t idx) noexcept {
	return _rowView.at(idx);
}

const DbCellView &DbRowView::operator[](std::size_t idx) const noexcept {
	return _rowView.at(idx);
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
