#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <uv_network/protocol.h>

class Server;

class ServerClient : public UVAbstractClient
{
public:
    ServerClient(Server *server);

private:
    void on_connect() override;
    void on_read(char *data, size_t size) override;
    void on_disconnect() override;

private:
    Server * const server;
    std::unique_ptr<Context> m_context;
};

#endif // SERVER_CLIENT_H
