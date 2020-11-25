#include "uv_tcp_handle.h"

UVTcpHandle::UVTcpHandle() :
    UVTcpHandle(uv_default_loop())
{
    auto sigint = new uv_signal_t;
    uv_signal_init(handle->loop, sigint);
    uv_signal_start(sigint, [](uv_signal_t *handle, int /*signum*/) {
        uv_walk(handle->loop, [](uv_handle_t *handle, void */*arg*/) {
            uv_close(handle, [](uv_handle_t *handle) {
                delete handle;
            });
        }, nullptr);
    }, SIGINT);
}

UVTcpHandle::UVTcpHandle(uv_loop_t *loop)
{
    handle = new uv_tcp_t;
    uv_tcp_init(loop, handle);
    uv_tcp_keepalive(handle, 1, 30);
    handle->data = this;
}

UVTcpHandle::~UVTcpHandle()
{
    auto handle = get_uv_handle();
    if (uv_is_closing(handle)) {
        delete handle;
    }
    else {
        uv_close(handle, [](uv_handle_t *handle) {
            delete handle;
        });
    }
}

uv_stream_t *UVTcpHandle::get_uv_stream()
{
    return reinterpret_cast<uv_stream_t *>(handle);
}

uv_handle_t *UVTcpHandle::get_uv_handle()
{
    return reinterpret_cast<uv_handle_t *>(handle);
}

uv_tcp_t *UVTcpHandle::get_uv_tcp()
{
    return handle;
}

uv_loop_t *UVTcpHandle::get_loop()
{
    return handle->loop;
}

bool UVTcpHandle::run_loop()
{
    return uv_run(handle->loop, UV_RUN_DEFAULT) == 0;
}

void UVTcpHandle::stop_loop()
{
    uv_stop(handle->loop);
}
