#include "client.h"
#include <uv_network/uv_buffers.h>
#include <iostream>
#include <filesystem>

int main(int argc, char **argv)
{
    if (argc < 4) {
        std::cout << "Usage: " << std::filesystem::path(argv[0]).filename().string()
                  << " host port source_file [bandwidth]\n" << std::endl;
        return 1;
    }
    if (!std::filesystem::exists(argv[3])) {
        std::cout << "file " << argv[3] << " doesn't exist" << std::endl;
        return 1;
    }
    const auto port = std::atoi(argv[2]);
    const auto host = argv[1];
    while (true) {
        std::cout << "Connecting to " << host << " " << port << std::endl;
        UVClient client(host, port, argv[3]);
        if (argc > 4) {
            client.set_bandwidth(std::atoll(argv[4]));
        }
        client.run_loop();
        if (client.is_done()) {
            UVBuffers::instance().clear();
            break;
        }
        std::cout << "Reconnecting" << std::endl;
    }
    std::cout << "Shutdown" << std::endl;
}
