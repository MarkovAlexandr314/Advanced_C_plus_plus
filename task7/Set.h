#include <unistd.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_set>

#ifndef SETH
#define SETH
template <class T>
struct Set;

template <class T>
struct SetImpl
{
    // Вставка
    virtual void insertImpl(T) = 0;
    // Удаление
    virtual void eraseImpl(T) = 0;
    // Проверка наличия
    virtual bool containsImpl(T) const = 0;
    // Размер
    virtual size_t sizeImpl() const = 0;
    // Получить все элементы seta
    virtual std::vector<T> getAllElements() const = 0;
    // Объединение set-ов
    virtual Set<T> mergeImpl(const Set<T> &) const = 0;
    // Пересечение set-ов
    virtual Set<T> intersectionImpl(const Set<T> &) const = 0;
};

template <typename T>
// Реализация множества на массиве
struct ArraySetImpl : public SetImpl<T>
{
    // Вставка
    void insertImpl(T value) override
    {
        if (!std::binary_search(arr.begin(), arr.end(), value))
        {
            arr.insert(std::upper_bound(arr.begin(), arr.end(), value), value);
        }
    }
    // Проверка наличия
    bool containsImpl(T value) const override
    {
        return std::binary_search(arr.begin(), arr.end(), value);
    }
    // Удаление
    void eraseImpl(T value) override
    {
        auto it = std::lower_bound(arr.begin(), arr.end(), value);
        if (it != arr.end() && *it == value)
        {
            arr.erase(it);
        }
    }
    // Размер
    size_t sizeImpl() const override
    {
        return arr.size();
    }
    // Получить все элементы seta
    std::vector<T> getAllElements() const override
    {
        return {arr.begin(), arr.end()};
    }
    // Объединение set-ов
    Set<T> mergeImpl(const Set<T> &other) const
    {
        Set<T> tmp;
        for (auto i : arr)
        {
            tmp.insert(i);
        }
        std::vector<T> all = other.getAllElements();
        for (auto i : all)
        {
            tmp.insert(i);
        }
        return tmp;
    }
    // Пересечение set-ов
    Set<T> intersectionImpl(const Set<T> &other) const
    {
        Set<T> tmp;
        for (auto i : arr)
        {
            if (other.contains(i))
                tmp.insert(i);
        }
        return tmp;
    }

private:
    std::vector<T> arr;
};

template <typename T>
// Реализация множества на хэш-таблице
struct HashSetImpl : SetImpl<T>
{
    // Вставка
    void insertImpl(T value) override
    {
        hash_table.insert(value);
    }
    // Проверка наличия
    bool containsImpl(T value) const override
    {
        return hash_table.find(value) != hash_table.end();
    }
    // Удаление
    void eraseImpl(T value) override
    {
        hash_table.erase(value);
    }
    // Размер
    size_t sizeImpl() const override
    {
        return hash_table.size();
    }
    // Получить все элементы seta
    std::vector<T> getAllElements() const override
    {
        return {hash_table.begin(), hash_table.end()};
    }
    // Объединение set-ов
    Set<T> mergeImpl(const Set<T> &other) const
    {
        Set<T> tmp;
        for (auto i : hash_table)
        {
            tmp.insert(i);
        }
        std::vector<T> all = other.getAllElements();
        for (auto i : all)
        {
            tmp.insert(i);
        }
        return tmp;
    }
    // Пересечение set-ов
    Set<T> intersectionImpl(const Set<T> &other) const
    {
        Set<T> tmp;
        for (auto i : hash_table)
        {
            if (other.contains(i))
                tmp.insert(i);
        }
        return tmp;
    }

private:
    std::unordered_set<T> hash_table;
};

template <class T>
struct Set
{
    // Изначально используем set на массиве
    Set() : set_impl(std::make_unique<ArraySetImpl<T>>()) {}
    // Получить размер
    size_t size() const
    {
        return set_impl->sizeImpl();
    }
    // Вставка
    void insert(T value)
    {
        set_impl->insertImpl(value);
        changeImpl();
    }
    // Удаление элемента
    void erase(T value)
    {
        set_impl->eraseImpl(value);
        changeImpl();
    }
    // Проверка наличия элемента
    bool contains(T value) const
    {
        return set_impl->containsImpl(value);
    }
    // Объединение множеств
    Set<T> merge(const Set<T> &value) const
    {
        return set_impl->mergeImpl(value);
    }
    // Пересечение множеств
    Set<T> intersection(const Set<T> &other) const
    {
        return set_impl->intersectionImpl(other);
    }
    // Получить все элементы
    std::vector<T> getAllElements() const
    {
        return set_impl->getAllElements();
    }

    SetImpl<T> *getImpl()
    {
        return set_impl.get();
    }

private:
    std::unique_ptr<SetImpl<T>> set_impl;
    // Пороговый размер
    const size_t max_size = 10;
    // Менять реализация в зависимости от размера
    void changeImpl()
    {
        // Set стал слишком большим, переносим из массива на хеш-таблицу
        if (set_impl->sizeImpl() > max_size && typeid(*set_impl.get()) == typeid(ArraySetImpl<T>))
        {
            auto elems = set_impl->getAllElements();
            auto tmp = std::make_unique<HashSetImpl<T>>();
            for (auto i : elems)
            {
                tmp->insertImpl(i);
            }
            set_impl = std::move(tmp);
        }
        // Set стал маленьким, переносим из хеш-таблицы в массив
        else if (set_impl->sizeImpl() <= max_size && typeid(*set_impl.get()) == typeid(HashSetImpl<T>))
        {
            auto elems = set_impl->getAllElements();
            auto tmp = std::make_unique<ArraySetImpl<T>>();
            for (auto i : elems)
            {
                tmp->insertImpl(i);
            }
            set_impl = std::move(tmp);
        }
    }
};
#endif // SETH