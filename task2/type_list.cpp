#include <iostream>
#include "type_list.h"

using namespace std;

class MyClass
{
};

int main()
{

    using MyList = detail::TypeList<int, int, double, char, detail::TypeList<int, double>, MyClass>;

    // Size
    static_assert(detail::Size<MyList>::value == 6);

    // TypeAt
    static_assert(std::is_same_v<detail::TypeAt<0, MyList>::type, int>);
    static_assert(std::is_same_v<detail::TypeAt<2, MyList>::type, double>);
    static_assert(std::is_same_v<detail::TypeAt<3, MyList>::type, char>);
    static_assert(std::is_same_v<detail::TypeAt<5, MyList>::type, MyClass>);

    // Contains
    static_assert(detail::Contains<int, MyList>::value);
    static_assert(detail::Contains<double, MyList>::value);
    static_assert(!detail::Contains<float, MyList>::value);
    static_assert(detail::Contains<MyClass, MyList>::value);

    // IndexOf
    static_assert(detail::IndexOf<int, MyList>::value == 0);
    static_assert(detail::IndexOf<double, MyList>::value == 2);
    static_assert(detail::IndexOf<char, MyList>::value == 3);
    static_assert(detail::IndexOf<float, MyList>::value == -1);
    static_assert(detail::IndexOf<detail::TypeList<int, double>, MyList>::value == 4);

    // AddFirst
    static_assert(std::is_same_v<detail::AddFirst<int, MyList>::type,
                                 detail::TypeList<int, int, int, double, char, detail::TypeList<int, double>, MyClass>>);
    static_assert(std::is_same_v<detail::AddFirst<detail::TypeList<MyClass>, MyList>::type,
                                 detail::TypeList<detail::TypeList<MyClass>, int, int, double, char, detail::TypeList<int, double>, MyClass>>);

    // AddLast
    static_assert(std::is_same_v<detail::AddLast<detail::TypeList<MyClass>, MyList>::type,
                                 detail::TypeList<int, int, double, char, detail::TypeList<int, double>, MyClass, detail::TypeList<MyClass>>>);
    static_assert(std::is_same_v<detail::AddLast<std::string, MyList>::type,
                                 detail::TypeList<int, int, double, char, detail::TypeList<int, double>, MyClass, std::string>>);

    // // using MyList = detail::TypeList<int, double, char, int, double, float>;
    // using MyList = detail::TypeList<>;
    // cout << "Size: " << detail::Size<MyList>::value << endl;
    // // cout << "At: " << typeid(detail::TypeAt<5, MyList>::type).name() << endl;
    // // cout << "At: " << std::is_same_v<detail::TypeAt<2, MyList>::type, char> << endl;
    // cout << "Contains: " << detail::Contains<MyClass, MyList>::value << endl;
    // cout << "IndexOf: " << detail::IndexOf<double, MyList>::value << endl;
}