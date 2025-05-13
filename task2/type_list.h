#include <cstdint>
#ifndef TYPELISTH
#define TYPELISTH
namespace detail
{

    template <class... Args>
    class TypeList
    {
    };

    /**
     * @brief Вспомогательный класс для получения размера
     *
     * @tparam T
     */
    template <class T>
    struct Size
    {
    };

    /**
     * @brief Специализация класс Size для TypeList
     *
     * @tparam Args
     */
    template <class... Args>
    struct Size<TypeList<Args...>>
    {
        static constexpr size_t value = sizeof...(Args);
    };

    /**
     * @brief Вспомогательный класс для получения элемента по индексуы
     *
     * @tparam N
     * @tparam args
     */
    template <size_t N, class T>
    struct TypeAt
    {
    };

    /**
     * @brief Рекурсивное получение по индексу.
     * Отшипываем первый элемент из Args и уменьшаем счетчик N
     *
     * @tparam N
     * @tparam Head
     * @tparam Args
     */
    template <size_t N, class Head, class... Args>
    struct TypeAt<N, TypeList<Head, Args...>>
    {
        using type = typename TypeAt<N - 1, TypeList<Args...>>::type;
    };

    /**
     * @brief Базовый случай, когда N=0.
     *
     * @tparam Head
     * @tparam Args
     */
    template <class Head, class... Args>
    struct TypeAt<0, TypeList<Head, Args...>>
    {
        using type = Head;
    };

    /**
     * @brief Класс для проверки наличия типа в списке
     *
     * @tparam T
     * @tparam Args
     */
    template <class T, class... Args>
    struct Contains
    {
    };

    template <class T, class Head, class... Args>
    struct Contains<T, TypeList<Head, Args...>>
    {
        static constexpr bool value = (std::is_same_v<Head, T>) || Contains<T, TypeList<Args...>>::value;
    };

    template <class T>
    struct Contains<T, TypeList<>>
    {
        static constexpr bool value = false;
    };

    // Получение индекса типа в списке
    template <class T, class... Args>
    class IndexOf
    {
    };
    // Рекурсивно проходмся по списку, отщипывая Head
    template <class T, class Head, class... Args>
    struct IndexOf<T, TypeList<Head, Args...>>
    {
    private:
        static constexpr int tmp_val = IndexOf<T, TypeList<Args...>>::value;

    public:
        static constexpr int value = (tmp_val == -1) ? -1 : tmp_val + 1;
    };
    // Нашли элемент в списке - аернули 0
    template <class T, class... Args>
    struct IndexOf<T, TypeList<T, Args...>>
    {
        static constexpr int value = 0;
    };
    // Список пуст - вернули -1
    template <class T>
    struct IndexOf<T, TypeList<>>
    {
        static constexpr int value = -1;
    };

    template <class T, class... Args>
    struct AddFirst
    {
    };

    template <class T, class... Args>
    struct AddFirst<T, TypeList<Args...>>
    {
        using type = TypeList<T, Args...>;
    };

    template <class T, class... Args>

    struct AddLast
    {
    };

    template <class T, class... Args>
    struct AddLast<T, TypeList<Args...>>
    {
        using type = TypeList<Args..., T>;
    };

}
#endif