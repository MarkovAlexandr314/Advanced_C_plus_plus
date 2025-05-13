#include <cstdint>

#ifndef MIXINH
#define MIXINH

template <class T>
class Counter
{
public:
    static size_t counter;

    Counter() { counter++; }
    static size_t count() { return counter; }
    ~Counter() { --counter; }
};

template <class T>
size_t Counter<T>::counter = 0;

template <typename Derived>
class LessThenComparable
{
    const Derived &self() const
    {
        return static_cast<const Derived &>(*this);
    }

public:
    bool operator>(const Derived &other) const
    {
        return other < self();
    }
    bool operator>=(const Derived &other) const
    {
        return !(self() < other);
    }
    bool operator<=(const Derived &other) const
    {
        return !(other < self());
    }
    bool operator==(const Derived &other) const
    {
        return !(other < self()) && !(self() < other);
    }
    bool operator!=(const Derived &other) const
    {
        return !(self() == other);
    }
};

class Number : public Counter<Number>, public LessThenComparable<Number>
{
public:
    Number(int value) : m_value{value} {}

    int value() const { return m_value; }

    bool operator<(Number const &other) const
    {
        return m_value < other.m_value;
    }

private:
    int m_value;
};
#endif // MIXINH