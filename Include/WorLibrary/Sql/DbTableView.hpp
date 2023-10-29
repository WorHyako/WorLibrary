#pragma once

#include "WorLibrary/Sql/DbRowView.hpp"

#include <map>
#include <optional>
#include <iterator>

namespace Wor::Sql {

    /**
     *
     */
    class DbTableView {
    public:
        /**
         * Ctor
         */
        DbTableView() = default;

    private:
        /**
         *
         */
        std::map<std::int64_t, DbRowView> _tableView;

    public:
#pragma region Accessors

        /**
         *
         * @param rowIndex
         * @return
         */
        [[nodiscard]] std::optional<DbRowView> GetRow(std::int64_t eventId) const noexcept;

        /**
         *
         * @return
         */
        [[nodiscard]] std::size_t Size() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

        /**
         *
         * @param row
         * @param overwrite
         * @return
         */
        [[nodiscard]] bool AddRow(DbRowView row, bool overwrite = false) noexcept;

#pragma endregion Mutators

#pragma region Operators

        [[nodiscard]] DbRowView &operator[](std::int64_t idx) noexcept;

        [[nodiscard]] std::map<std::int64_t, DbRowView>::iterator begin() noexcept;

        [[nodiscard]] std::map<std::int64_t, DbRowView>::const_iterator begin() const noexcept;

        [[nodiscard]] std::map<std::int64_t, DbRowView>::iterator end() noexcept;

        [[nodiscard]] std::map<std::int64_t, DbRowView>::const_iterator end() const noexcept;

        [[nodiscard]] std::map<std::int64_t, DbRowView>::const_iterator cbegin() const noexcept;

        [[nodiscard]] std::map<std::int64_t, DbRowView>::const_iterator cend() const noexcept;

#pragma endregion Operators
    };
}
