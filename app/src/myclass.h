#pragma once

class MyClass
{
public:
    MyClass();
    MyClass(int first, int second);
    int add() const;
    int multiply() const;

private:
    int m_firstValue;
    int m_secondValue;
};
