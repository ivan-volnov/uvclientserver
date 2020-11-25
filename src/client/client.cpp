#include "client.h"
#include <iostream>


UVClient::UVClient(const char *ip, int port, const std::string &filename, uv_loop_t *loop) :
    UVAbstractTcpClient(ip, port, loop),
    m_context(std::make_unique<Context>(this)),
    m_filename(filename)
{

}

void UVClient::set_bandwidth(uint64_t value)
{
    m_bandwidth = value;
}

bool UVClient::is_done() const
{
    return m_context->is_done();
}

void UVClient::on_connect()
{
    std::cout << "Client       : connected" << std::endl;
    std::cout << "Client       : sending file " << m_filename;
    if (m_bandwidth) {
        std::cout << " at " << m_bandwidth << "MBit/s";
    }
    std::cout << std::endl;
    m_context->switch_to(std::make_unique<StateClientSendFile>(m_filename, m_bandwidth));
}

void UVClient::on_read(char *data, size_t size)
{
    m_context->handle_request(data, data + size);
}

void UVClient::on_disconnect()
{
    std::cout << "Client       : disconnected" << std::endl;
}
