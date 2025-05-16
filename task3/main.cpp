#include "TypeMap.h"
#include "iostream"

struct DataA
{
    std::string value;
};

struct DataB
{
    int value;
};

int main()
{
    TypeMap<int, DataA, double, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>({"Hello, TypeMap!"});
    myTypeMap.AddValue<DataB>({10});

    std::cout << "Value for int: " << myTypeMap.GetValue<int>() << std::endl; // 42
    std::cout << "Value for double: " << myTypeMap.GetValue<double>() << std::endl; // 3.14
    std::cout << "Value for DataA: " << myTypeMap.GetValue<DataA>().value << std::endl; // Hello, TypeMap!
    std::cout << "Value for DataB: " << myTypeMap.GetValue<DataB>().value << std::endl; // 10

    std::cout << "Contains int? " << (myTypeMap.Contains<int>() ? "Yes" : "No") << std::endl; // Yes

    // Удаление
    myTypeMap.RemoveValue<double>();
    // Попытка обрашения к удаленному элементу
    try {
        std::cout << myTypeMap.GetValue<double>() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl; // Вывод: Error: Element not present
    }

    return 0;
}