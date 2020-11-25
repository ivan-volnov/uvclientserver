#include "server.h"
#include <uv_network/uv_buffers.h>
#include <iostream>
#include <filesystem>

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "Usage: " << std::filesystem::path(argv[0]).filename().string()
                  << " host port" << std::endl;
        return 1;
    }
    const auto port = std::atoi(argv[2]);
    const auto host = argv[1];
    {
        std::cout << "Listening " << host << " " << port << std::endl;
        Server server(host, port);
        server.run_loop();
        UVBuffers::instance().clear();
    }
    std::cout << "Shutdown" << std::endl;
}
