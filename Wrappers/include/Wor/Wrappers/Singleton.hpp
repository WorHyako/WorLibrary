#pragma once

namespace Wor::Wrappers {

    /**
     * @brief   Just casual singleton wrapper.
     *
     * @usage
     * @code
     *          auto &instance = Singleton<InstanceType>::get();
     * @endcode
     *
     * @author  WorHyako
     */
    template<typename TInstanceType>
    class Singleton final
            : private TInstanceType {
    public:
        /**
         * @brief Ctor.
         */
        Singleton() noexcept = delete;

        /**
         * @brief Copy ctor.
         */
        Singleton(const Singleton &) = delete;

        [[nodiscard]]
        Singleton &operator=(const Singleton) = delete;

    public:
#pragma region Accessors/Mutators

        /**
         * @brief
         *
         * @return
         */
        [[nodiscard]]
        static TInstanceType &get() noexcept;

#pragma endregion Accessors/Mutators
    };

#pragma region Accessors/Mutators

    template<typename TInstanceType>
    TInstanceType &Singleton<TInstanceType>::get() noexcept {
        static TInstanceType instance;
        return instance;
    }

#pragma endregion Accessors/Mutators
}
