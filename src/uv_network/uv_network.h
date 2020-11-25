#ifndef UV_NETWORK_H
#define UV_NETWORK_H

#include "uv_tcp_handle.h"

class UVAbstractClient;
class UVAbstractTcpClient;

class UVAbstractServer : public UVTcpHandle
{
public:
    UVAbstractServer(const char *ip, int port);

protected:
    virtual UVAbstractClient *create_client() = 0;
};



class UVAbstractClient : public UVTcpHandle
{
    friend class UVAbstractServer;
    friend class UVAbstractTcpClient;

public:
    UVAbstractClient(uv_loop_t *loop);

public:
    void close();
    void write(uv_buf_t &&buf, size_t size);
    static uv_buf_t get_buffer(size_t size = 65536);

protected:
    virtual void on_connect() = 0;
    virtual void on_read(char *data, size_t size) = 0;
    virtual void on_disconnect() = 0;

private:
    void start_reading();
};



class UVAbstractTcpClient : public UVAbstractClient
{
public:
    UVAbstractTcpClient(const char *ip, int port, uv_loop_t *loop);

private:
    uv_connect_t *m_connection;
};

#endif // UV_NETWORK_H
