#include <iostream>
#include <cassert>
#include "Set.h"

void test_basic_operations()
{
    Set<int> set;
    // Тест пустого множества
    assert(set.size() == 0);
    assert(!set.contains(5));

    // Добавление элементов
    set.insert(1);
    set.insert(2);
    set.insert(3);
    assert(set.size() == 3);
    assert(set.contains(2));

    // Добавление дубликата
    set.insert(2);
    assert(set.size() == 3);

    // Удаление элемента
    set.erase(2);
    assert(set.size() == 2);
    assert(!set.contains(2));
}

void test_set_operations()
{
    Set<int> s1, s2;

    // Заполняем множества
    for (int i = 1; i <= 10; ++i)
        s1.insert(i);
    for (int i = 5; i <= 15; ++i)
        s2.insert(i);

    // Объединение
    auto union_set = s1.merge(s2);
    assert(union_set.size() == 15);
    for (int i = 1; i <= 15; ++i)
        assert(union_set.contains(i));

    // Пересечение
    auto intersect_set = s1.intersection(s2);
    assert(intersect_set.size() == 6);
    for (int i = 5; i <= 10; ++i)
        assert(intersect_set.contains(i));
}

void test_implementation_switching()
{
    Set<int> set;

    // Изначально должна быть array-реализация
    assert(typeid(*set.getImpl()) == typeid(ArraySetImpl<int>));

    // Добавляем элементы за порог
    for (int i = 0; i < 15; ++i)
        set.insert(i);
    assert(typeid(*set.getImpl()) == typeid(HashSetImpl<int>));

    // Удаляем элементы ниже порога
    for (int i = 0; i < 10; ++i)
        set.erase(i);
    assert(typeid(*set.getImpl()) == typeid(ArraySetImpl<int>));
}

int main()
{
    try
    {
        test_basic_operations();
        test_set_operations();
        test_implementation_switching();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}