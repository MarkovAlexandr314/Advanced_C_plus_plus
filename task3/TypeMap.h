#include "../task2/type_list.h"
#include <tuple>
#include <stdexcept> // Для std::runtime_error

template <typename... Types>
class TypeMap {
private:
    template <typename T>
    struct Element {
        T value{};
        bool is_present = false;
    };

    using Storage = std::tuple<Element<Types>...>;
    Storage storage_;

    using List = detail::TypeList<Types...>;

    template <typename T>
    static constexpr size_t GetIndex() {
        return detail::IndexOf<T, List>::value;
    }

public:
    template <typename T>
    void AddValue(T value) {
        static_assert(GetIndex<T>() != static_cast<size_t>(-1), "Type not in TypeMap");
        auto& elem = std::get<GetIndex<T>()>(storage_);
        elem.value = value;
        elem.is_present = true;
    }

    template <typename T>
    T& GetValue() {
        static_assert(GetIndex<T>() != static_cast<size_t>(-1), "Type not in TypeMap");
        auto& elem = std::get<GetIndex<T>()>(storage_);
        if (!elem.is_present) {
            throw std::runtime_error("Element not present");
        }
        return elem.value;
    }

    template <typename T>
    const T& GetValue() const {
        static_assert(GetIndex<T>() != static_cast<size_t>(-1), "Type not in TypeMap");
        const auto& elem = std::get<GetIndex<T>()>(storage_);
        if (!elem.is_present) {
            throw std::runtime_error("Element not present");
        }
        return elem.value;
    }

    template <typename T>
    bool Contains() const {
        static_assert(GetIndex<T>() != static_cast<size_t>(-1), "Type not in TypeMap");
        return std::get<GetIndex<T>()>(storage_).is_present;
    }

    template <typename T>
    void RemoveValue() {
        static_assert(GetIndex<T>() != static_cast<size_t>(-1), "Type not in TypeMap");
        auto& elem = std::get<GetIndex<T>()>(storage_);
        elem.is_present = false;
        elem.value = T{};
    }
};