#pragma once

namespace Wor::TemplateWrapper {

    /**
     * @brief   Just casual singleton wrapper.
     *
     * @usage
     * @code
     *          auto &instance = Singleton<InstanceType>::initInstance(arg0, arg1, ...);
     *
     *          auto &instance = Singleton<InstanceType>::getInstance();
     * @endcode
     *
     * @author  WorHyako
     */
    template<typename TInstanceType>
    class Singleton final
            : private TInstanceType {
    public:
        /**
         * @brief Dtor.
         */
        ~Singleton() noexcept = delete;

        /**
         * @brief Ctor.
         */
        Singleton() noexcept = delete;

        /**
         * @brief Ctor.
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
