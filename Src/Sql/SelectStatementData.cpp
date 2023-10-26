#include "WorLibrary/Sql/SelectStatementData.hpp"

#include <sstream>
#include <algorithm>

using namespace Wor::Sql;

std::string SelectStatementData::ToString() const noexcept {
    if (selectValues.empty()) {
        return std::string {};
    }

    std::stringstream ss;

    ss << "SELECT ";
    std::for_each(selectValues.begin(), selectValues.end() - 1, [&ss](const auto &select) {
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
