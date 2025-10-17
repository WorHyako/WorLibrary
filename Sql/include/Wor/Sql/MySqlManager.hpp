#pragma once

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

#include "Wor/Sql/DataBaseParameters.hpp"
#include "Wor/Sql/Event/SelectStatementData.hpp"
#include "Wor/Sql/DbTableView.hpp"

#include <string>

namespace Wor::Sql {
	/**
	 * @brief
	 *
	 * @author WorHyako
	 */
	class MySqlManager {
	public:
		/**
		 * @brief
		 *
		 * @author WorHyako
		 */
		enum class ConnectionStatus
				: std::uint8_t {
			/**
			 *
			 */
			ZeroCheck,

			/**
			 *
			 */
			Connected,

			/**
			 *
			 */
			AuthFailed,

			/**
			 *
			 */
			Unreachable
		};

		/**
		 * @brief Ctor.
		 *
		 * @param dbParameters
		 */
		explicit MySqlManager(DataBaseParameters dbParameters = {}) noexcept;

		/**
		 * @brief
		 *
		 * @param dbParameters
		 */
		void configure(DataBaseParameters dbParameters) noexcept;

		/**
		 * @brief
		 *
		 * @param connectionString
		 *
		 * @return
		 */
		[[nodiscard]]
		ConnectionStatus tryToConnect() noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		DbTableView select(const Event::SelectStatementData& statementData) noexcept;

	private:
		/**
		 * Manager current status
		 */
		ConnectionStatus _status;

		/**
		 * Current data base session
		 */
		soci::session _session;

		/**
		 * Data Base parameters for connection string
		 */
		DataBaseParameters _dbParameters;

	public:
#pragma region Accessors/Mutators

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		ConnectionStatus status() const noexcept;

		/**
		 * @brief
		 *
		 * @return
		 */
		[[nodiscard]]
		const DataBaseParameters& dpParameters() const noexcept;

		/**
		 * @brief
		 *
		 * @param dpParameters
		 *
		 * @return
		 */
		[[nodiscard]]
		bool dpParameters(DataBaseParameters dpParameters) noexcept;

#pragma endregion Accessors/Mutators
	};
}
