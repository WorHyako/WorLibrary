#pragma once

namespace boost::asio {
    class io_context;
}

namespace Wor::Network::Utils::IoService {

    /**
     * @brief
     *
     * @return
     *
     * @author WorHyako
     */
    [[nodiscard]]
    boost::asio::io_context& get() noexcept;

    /**
     * @brief
     *
     * @author WorHyako
     */
    void run() noexcept;

    /**
     * @brief
     *
     * @author WorHyako
     */
    [[nodiscard]]
    bool isRunning() noexcept;
}
