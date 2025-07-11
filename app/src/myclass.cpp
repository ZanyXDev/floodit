#include "myclass.h"

MyClass::MyClass() { }

MyClass::MyClass(int first, int second) : m_firstValue(first), m_secondValue(second) { }

int MyClass::add() const
{
    return m_firstValue + m_secondValue;
}

int MyClass::multiply() const
{
    return m_firstValue * m_secondValue;
}
