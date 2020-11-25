#ifndef STACK_H
#define STACK_H

#include <utility>

template<class T, size_t SIZE>
class Stack
{
public:
    Stack() = default;

    T &operator[] (size_t idx)
    {
        return _stack[idx];
    }

    const T &operator[] (size_t idx) const
    {
        return _stack[idx];
    }

    T *begin()
    {
        return empty() ? nullptr : _stack;
    }

    T *end()
    {
        return empty() ? nullptr : &_stack[_next];
    }

    void push(const T &value)
    {
        _stack[_next++] = value;
    }

    void push(T &&value)
    {
        _stack[_next++] = std::forward<T>(value);
    }

    T &take()
    {
        return _stack[--_next];
    }

    void pop()
    {
        --_next;
    }

    T &top()
    {
        return _stack[_next - 1];
    }

    bool haveSpace() const
    {
        return _next < SIZE;
    }

    bool empty() const
    {
        return not _next;
    }

    size_t size() const
    {
        return _next;
    }

private:
    T _stack[SIZE];
    size_t _next = 0;
};

#endif // STACK_H
