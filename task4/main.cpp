#include <iostream>
#include <cassert>
#include "MixIn.h"

using namespace std;

int main()
{
    Number one{1};
    Number two{2};
    {
        Number a{0};
    }
    Number three{3};
    Number four{4};
    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);
    assert(one != two);
    std::cout << "Count: " << Counter<Number>::count() << std::endl;

    return 0;
}