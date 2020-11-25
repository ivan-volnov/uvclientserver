#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>



template<typename T>
class ByteParser
{
public:
    bool parse(char ch)
    {
        if (m_pos < sizeof(T)) {
            reinterpret_cast<char *>(&m_value)[m_pos++] = ch;
            return is_valid();
        }
        return false;
    }

    bool is_valid()
    {
        return m_pos == sizeof(T);
    }

    void reset()
    {
        m_pos = 0;
    }

    const T &value() const
    {
        return m_value;
    }

private:
    size_t m_pos = 0;
    T m_value;
};



template<typename T>
class ArrayParser
{
public:
    bool parse(typename T::value_type ch)
    {
        if (m_count > 0) {
            m_value.push_back(ch);
            return !--m_count;
        }
        return false;
    }

    bool is_valid()
    {
        return !m_count;
    }

    void reset(size_t size)
    {
        m_count = size;
        m_value.clear();
    }

    const T &value() const
    {
        return m_value;
    }

private:
    size_t m_count = 0;
    T m_value;
};



#endif // PARSER_H
