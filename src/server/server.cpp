#include "server.h"
#include <iostream>
#include "server_client.h"

Server::Server(const char *ip, int port) :
    UVAbstractServer(ip, port)
{
}

Server::~Server() = default;


UVAbstractClient *Server::create_client()
{
    return new ServerClient(this);
}

void Server::on_client_connect(ServerClient */*client*/)
{
    std::cout << "Server       : client connected" << std::endl;
}

void Server::on_client_disconnect(ServerClient */*client*/)
{
    std::cout << "Server       : client disconnected" << std::endl;

#ifdef TESTING
    stop_loop();  // just for testing
#endif
}
