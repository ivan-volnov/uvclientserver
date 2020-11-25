#ifndef UV_SERVER_H
#define UV_SERVER_H

#include <uv_network/uv_network.h>

class ServerClient;

class Server : public UVAbstractServer
{
    friend class ServerClient;
public:
    Server(const char *ip, int port);
    ~Server() override;

    Server(const Server &) = delete;
    Server(Server &&) = delete;
    Server &operator = (const Server &) = delete;
    Server &operator = (Server &&) = delete;

private:
    UVAbstractClient *create_client() override;

    void on_client_connect(ServerClient *client);
    void on_client_disconnect(ServerClient *client);
};

#endif // UV_SERVER_H
