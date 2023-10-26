#pragma once

#include "soci/soci.h"

#include <sstream>
#include <string>

namespace Wor::Sql::Utils {

    /**
     *
     */
    class DataConverter final {
    public:

        /**
         *
         * @tparam T
         * @param row
         * @param cellIndex
         * @return
         */
        static std::string SociToString(const soci::row &row, int cellIndex) noexcept;
    };

    inline std::string DataConverter::SociToString(const soci::row &row, int cellIndex) noexcept {
        std::string s {};
        if (cellIndex > (row.size() - 1)) {
            return s;
        }
        const soci::column_properties &property = row.get_properties(cellIndex);
        switch (property.get_data_type()) {
            case soci::dt_double:
                s = std::to_string(row.get<double>(cellIndex));
                break;
            case soci::dt_integer:
                s = std::to_string(row.get<int>(cellIndex));
                break;
            case soci::dt_string:
                s = row.get<std::string>(cellIndex);
                break;
            case soci::dt_long_long:
                s = std::to_string(row.get<long long>(cellIndex));
                break;
            default:
                break;
        }
        return s;
    }
}
