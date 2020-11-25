#ifndef UV_TCP_HANDLE_H
#define UV_TCP_HANDLE_H

#include <uv.h>

class UVTcpHandle
{
public:
    UVTcpHandle();
    UVTcpHandle(uv_loop_t *loop);
    virtual ~UVTcpHandle();

    UVTcpHandle(const UVTcpHandle &) = delete;
    UVTcpHandle(UVTcpHandle &&) = delete;
    UVTcpHandle &operator=(const UVTcpHandle &) = delete;
    UVTcpHandle &operator=(UVTcpHandle &&) = delete;

public:
    uv_stream_t *get_uv_stream();
    uv_handle_t *get_uv_handle();
    uv_tcp_t *get_uv_tcp();
    uv_loop_t *get_loop();

    bool run_loop();
    void stop_loop();

private:
    uv_tcp_t *handle;
};

#endif // UV_TCP_HANDLE_H
