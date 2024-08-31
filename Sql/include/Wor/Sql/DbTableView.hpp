#pragma once

#include "Wor/Sql/DbRowView.hpp"

#include <optional>
#include <iterator>
#include <cstdint>

namespace Wor::Sql {
	class DbRowView;

	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class DbTableView {
	public:
		/**
		 * @brief Ctor.
		 */
		DbTableView() = default;

		/**
		 * @brief
		 *
		 * @param property
		 */
		void sort(const std::string& property = "EventID") noexcept;

	private:
		/**
		 * @brief
		 */
		std::vector<DbRowView> _tableView;

		/**
		 * @brief
		 *
		 * @param element
		 *
		 * @return
		 */
		[[nodiscard]]
		std::vector<DbRowView>::iterator find(const DbRowView& element) const noexcept;

		/**
		 * @brief
		 *
		 * @param eventId
		 *
		 * @return
		 */
		[[nodiscard]]
		std::vector<DbRowView>::iterator find(std::int64_t eventId) const noexcept;

	public:
#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @param rowIndex
		 *
		 * @return
		 */
		[[nodiscard]]
		std::optional<DbRowView> getRow(std::int64_t eventId) const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		std::size_t size() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		const DbRowView& back() const noexcept;

		/**
		 * @brief
		 *
		 * @param row
		 *
		 * @param overwrite
		 */
		void AddRow(DbRowView row, bool overwrite = false) noexcept;

#pragma endregion Accessors/Mutators

#pragma region Operators

		[[nodiscard]]
		DbRowView& operator[](std::int64_t idx) noexcept;

		[[nodiscard]]
		const DbRowView& operator[](std::int64_t idx) const noexcept;

		[[nodiscard]]
		std::vector<DbRowView>::iterator begin() noexcept;

		[[nodiscard]]
		std::vector<DbRowView>::const_iterator begin() const noexcept;

		[[nodiscard]]
		std::vector<DbRowView>::iterator end() noexcept;

		[[nodiscard]]
		std::vector<DbRowView>::const_iterator end() const noexcept;

		[[nodiscard]]
		std::vector<DbRowView>::const_iterator cbegin() const noexcept;

		[[nodiscard]]
		std::vector<DbRowView>::const_iterator cend() const noexcept;

#pragma endregion Operators
	};
}
