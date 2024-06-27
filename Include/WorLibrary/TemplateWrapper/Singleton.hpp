#pragma once

namespace Wor::TemplateWrapper {

    /**
     * @brief
     *
     * @author WorHyako
     */
    template<typename T>
    class Singleton final
            : private T {
    public:
        /**
         * @brief Ctor.
         */
        Singleton(const Singleton &) = delete;

        Singleton &operator=(const Singleton) = delete;

    private:
        /**
         * @brief Dtor.
         */
        ~Singleton() noexcept = default;

        /**
         * @brief Ctor.
         */
        Singleton() noexcept = default;

    public:
#pragma region Accessors

        /**
         * @brief
         *
         * @return  Static object instance
         */
        static T &getInstance();

#pragma endregion Accessors
    };

#pragma region Accessors

    template<typename T>
    T &Singleton<T>::getInstance() {
        static Singleton instance {};
        return instance;
    }

#pragma endregion Accessors
}
