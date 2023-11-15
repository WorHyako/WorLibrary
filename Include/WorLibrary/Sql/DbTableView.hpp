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

        /**
         *
         * @param property
         */
        void Sort(const std::string& property = "EventID") noexcept;

    private:
        /**
         *
         */
        std::vector<DbRowView> _tableView;

        /**
         *
         * @param element
         * @return
         */
        [[nodiscard]] std::vector<DbRowView>::iterator find(const DbRowView& element) const noexcept;

        /**
         *
         * @param eventId
         * @return
         */
        [[nodiscard]] std::vector<DbRowView>::iterator find(std::int64_t eventId) const noexcept;

    public:
#pragma region Accessors

        /**
         *
         * @param rowIndex
         * @return
         */
        [[nodiscard]] std::optional<DbRowView> getRow(std::int64_t eventId) const noexcept;

        /**
         *
         * @return
         */
        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] const DbRowView& back() const noexcept;

#pragma endregion Accessors

#pragma region Mutators

        /**
         *
         * @param row
         * @param overwrite
         */
        void AddRow(DbRowView row, bool overwrite = false) noexcept;

#pragma endregion Mutators

#pragma region Operators

        [[nodiscard]] DbRowView &operator[](std::int64_t idx) noexcept;

        [[nodiscard]] const DbRowView &operator[](std::int64_t idx) const noexcept;

        [[nodiscard]] std::vector<DbRowView>::iterator begin() noexcept;

        [[nodiscard]] std::vector<DbRowView>::const_iterator begin() const noexcept;

        [[nodiscard]] std::vector<DbRowView>::iterator end() noexcept;

        [[nodiscard]] std::vector<DbRowView>::const_iterator end() const noexcept;

        [[nodiscard]] std::vector<DbRowView>::const_iterator cbegin() const noexcept;

        [[nodiscard]] std::vector<DbRowView>::const_iterator cend() const noexcept;

#pragma endregion Operators
    };
}
