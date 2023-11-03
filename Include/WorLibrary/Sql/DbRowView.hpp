#pragma once

#include "WorLibrary/Sql/DbCellView.hpp"

#include <vector>
#include <iterator>

namespace Wor::Sql {

    /**
     *
     */
    class DbRowView {
    public:

        /**
         *
         * @param cell
         */
        void Add(DbCellView cell) noexcept;

        /**
         *
         * @param cellName
         * @return
         */
        [[nodiscard]] std::string Find(const std::string& cellName) const noexcept;

    private:
        /**
         * 
         */
        std::vector<DbCellView> _rowView;

    public:
#pragma region Operators

        [[nodiscard]] DbCellView &operator[](std::size_t idx) noexcept;

        [[nodiscard]] const DbCellView &operator[](std::size_t idx) const noexcept;

        [[nodiscard]] std::vector<DbCellView>::iterator begin() noexcept;

        [[nodiscard]] std::vector<DbCellView>::const_iterator begin() const noexcept;

        [[nodiscard]] std::vector<DbCellView>::iterator end() noexcept;

        [[nodiscard]] std::vector<DbCellView>::const_iterator end() const noexcept;

        [[nodiscard]] std::vector<DbCellView>::const_iterator cbegin() const noexcept;

        [[nodiscard]] std::vector<DbCellView>::const_iterator cend() const noexcept;

#pragma endregion Operators
    };
}