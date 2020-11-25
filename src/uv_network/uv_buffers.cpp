#include "uv_buffers.h"

UVBuffers::~UVBuffers()
{
    clear();
}

UVBuffers &UVBuffers::instance()
{
    static UVBuffers instance;
    return instance;
}

void UVBuffers::store(uv_buf_t &&buf)
{
    if (buf.len < 65536) {
        delete[] buf.base;
        return;
    }
    if (buffers.haveSpace()) { // 2048 * >=65536 bytes
        buffers.push(buf);
    }
    else {
        delete[] buffers.top().base;
        buffers.top() = buf;
    }
}

uv_buf_t UVBuffers::take(size_t size)
{
    if (buffers.empty() || buffers.top().len < size) {
        return uv_buf_init(new char[size], size);
    }
    return buffers.take();
}

void UVBuffers::clear()
{
    while (!buffers.empty()) {
        delete[] buffers.take().base;
    }
}
