#include "server_client.h"
#include <iostream>
#include "server.h"



ServerClient::ServerClient(Server *server) :
    UVAbstractClient(server->get_loop()),
    server(server),
    m_context(std::make_unique<Context>(this))
{
    m_context->switch_to(std::make_unique<StateServerParseInitPacket>());
}

void ServerClient::on_connect()
{
    server->on_client_connect(this);
    std::cout << "Server client: connected" << std::endl;
}

void ServerClient::on_read(char *data, size_t size)
{
    m_context->handle_request(data, data + size);
}

void ServerClient::on_disconnect()
{
    std::cout << "Server client: disconnected" << std::endl;
    server->on_client_disconnect(this);
    delete this;
}
