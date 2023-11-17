#include "WorLibrary/Sql/Event/SelectStatementData.hpp"

#include <sstream>
#include <algorithm>

using namespace Wor::Sql::Event;

std::string SelectStatementData::toString() const noexcept {
    if (selectValues.empty()) {
        return std::string {};
    }

    std::stringstream ss;

    ss << "SELECT ";
    std::for_each(std::begin(selectValues), std::end(selectValues) - 1, [&ss](const auto &select) {
        ss << select << ", ";
    });
    ss << selectValues.back() << " ";

    if (!fromValue.empty()) {
        ss << "FROM " << fromValue << " ";
    }

    if (!whereValue.empty()) {
        ss << "WHERE " << whereValue << " ";
    }

    if (!orderByValue.empty()) {
        ss << "ORDER BY " << orderByValue << " ";
    }
    return ss.str();
}
