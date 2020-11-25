#ifndef UV_BUFFERS_H
#define UV_BUFFERS_H

#include "../utility/stack.h"
#include <uv.h>


class UVBuffers
{
public:
    ~UVBuffers();

    UVBuffers(const UVBuffers &) = delete;
    UVBuffers(UVBuffers &&) = delete;
    UVBuffers &operator = (const UVBuffers &) = delete;
    UVBuffers &operator = (UVBuffers &&) = delete;

    static UVBuffers &instance();

public:
    void store(uv_buf_t &&buf);
    uv_buf_t take(size_t size = 65536);
    void clear();

private:
    UVBuffers() = default;

    Stack<uv_buf_t, 2048> buffers;
};

#endif // UV_BUFFERS_H
