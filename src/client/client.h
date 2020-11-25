#ifndef UV_CLIENT_H
#define UV_CLIENT_H

#include <uv_network/protocol.h>


class UVClient : public UVAbstractTcpClient
{
public:
    UVClient(const char *ip, int port, const std::string &filename, uv_loop_t *loop = uv_default_loop());

    void set_bandwidth(uint64_t value);

    bool is_done() const;

private:
    void on_connect() override;
    void on_read(char *data, size_t size) override;
    void on_disconnect() override;

private:
    std::unique_ptr<Context> m_context;
    std::string m_filename;
    uint64_t m_bandwidth = 0;
};

#endif // UV_CLIENT_H
