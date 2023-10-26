#pragma once

namespace Wor::TemplateWrapper {

    /**
     *
     */
    template<typename T>
    class Singleton final
            : private T {
    public:
        /**
         * Ctor
         */
        Singleton(const Singleton &) = delete;

        Singleton &operator=(const Singleton) = delete;

    private:
        /**
         * Dtor
         */
        ~Singleton() noexcept = default;

        /**
         * Ctor
         */
        Singleton() noexcept = default;

    public:
#pragma region Accessors

        /**
         *
         * @return  Static object instance
         */
        static T &GetInstance();

#pragma endregion Accessors
    };

#pragma region Accessors

    template<typename T>
    T &Singleton<T>::GetInstance() {
        static Singleton instance {};
        return instance;
    }

#pragma endregion Accessors
}
