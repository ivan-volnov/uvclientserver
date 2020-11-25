#include "uv_network.h"
#include "uv_buffers.h"
#include <exception>
#include <iostream>

UVAbstractServer::UVAbstractServer(const char *ip, int port)
{
    constexpr int backlog = 128;
    struct sockaddr_in addr{};
    uv_ip4_addr(ip, port, &addr);
    uv_tcp_bind(get_uv_tcp(), reinterpret_cast<const struct sockaddr *>(&addr), 0);
    if (uv_listen(get_uv_stream(), backlog, [](uv_stream_t *server_stream, int status) {
        if (status == -1) {
            return;
        }
        auto server = reinterpret_cast<UVAbstractServer *>(server_stream->data);
        auto client = server->create_client();
        if (uv_accept(server_stream, client->get_uv_stream()) == 0) {
            client->on_connect();
            client->start_reading();
        }
        else {
            uv_close(client->get_uv_handle(), [](auto handle) {
                delete reinterpret_cast<UVAbstractServer *>(handle->data);
            });
        }
    }) != 0) {
        throw std::exception();
    }
}

UVAbstractClient::UVAbstractClient(uv_loop_t *loop) :
    UVTcpHandle(loop)
{

}

void UVAbstractClient::close()
{
    if (uv_is_closing(get_uv_handle())) {
        on_disconnect();
    }
    else {
        uv_close(get_uv_handle(), [](auto handle) {
            auto instance = reinterpret_cast<UVAbstractClient *>(handle->data);
            instance->on_disconnect();
        });
    }
}

void UVAbstractClient::write(uv_buf_t &&buf, size_t size)
{
    struct WriteRequest
    {
        uv_buf_t buf;
        size_t buffer_size;
        uv_write_t handle;
    };
    // set buf.len to size because it's the only way to tell uv_write the length of data to write
    auto req = new WriteRequest{{ buf.base, size }, buf.len, {} };
    req->handle.data = req;
    uv_write(&req->handle, get_uv_stream(), &req->buf, 1, [](uv_write_t *req, int status) {
        if (status == -1) {
            std::cout << "Socket write error " << uv_strerror(status) << std::endl;
        }
        auto request = reinterpret_cast<WriteRequest *>(req->data);
        UVBuffers::instance().store({ request->buf.base, request->buffer_size });
        delete request;
    });
}

uv_buf_t UVAbstractClient::get_buffer(size_t size)
{
    return UVBuffers::instance().take(size);
}

void UVAbstractClient::start_reading()
{
    uv_read_start(get_uv_stream(), [](uv_handle_t *, size_t size, uv_buf_t *buf) {
        *buf = get_buffer(size);
    }, [](uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
        auto instance = reinterpret_cast<UVAbstractClient *>(stream->data);
        if (nread > 0) {
            instance->on_read(buf->base, static_cast<size_t>(nread));
            UVBuffers::instance().store(std::move(*const_cast<uv_buf_t *>(buf)));
        }
        else if (nread < 0) {
            if (nread != UV_EOF) {
                std::cout << "Socket read error " << uv_strerror(nread) << std::endl;
            }
            instance->close();
        }
    });
}



UVAbstractTcpClient::UVAbstractTcpClient(const char *ip, int port, uv_loop_t *loop) :
    UVAbstractClient(loop), m_connection(new uv_connect_t)
{
    m_connection->data = this;
    struct sockaddr_in addr{};
    uv_ip4_addr(ip, port, &addr);
    uv_tcp_connect(m_connection, get_uv_tcp(), reinterpret_cast<const struct sockaddr *>(&addr),
        [](uv_connect_t *connection, int status) {
            auto instance = reinterpret_cast<UVAbstractClient *>(connection->data);
            if (status < 0) {
                instance->close();
            }
            else {
                instance->on_connect();
                instance->start_reading();
            }
        }
    );
}
