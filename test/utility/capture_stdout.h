#ifndef CAPTURE_STDOUT_H
#define CAPTURE_STDOUT_H

#include <iostream>
#include <sstream>


class CaptureStdout
{
public:
    CaptureStdout() :
        m_sbuf(std::cout.rdbuf())
    {
        std::cout.rdbuf(m_ss.rdbuf());
    }

    ~CaptureStdout()
    {
        std::cout.rdbuf(m_sbuf);
    }

    CaptureStdout(const CaptureStdout &) = delete;
    CaptureStdout &operator=(const CaptureStdout &) = delete;

public:
    std::string data() const
    {
        return m_ss.str();
    }

    void clear()
    {
        std::stringstream ss;
        std::swap(m_ss, ss);
        std::cout.rdbuf(m_ss.rdbuf());
    }

private:
    std::stringstream m_ss;
    std::streambuf *m_sbuf;
};


#endif // CAPTURE_STDOUT_H
