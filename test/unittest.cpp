#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <uv_network/uv_buffers.h>
#include <server/server.h>
#include <client/client.h>
#include <uv_network/protocol.h>
#include "utility/capture_stdout.h"


TEST_CASE("UVClientServer")
{
    const auto host = "127.0.0.1";
    const auto port = 9000;

    CaptureStdout stdout;

    Server server(host, port);
    UVClient client(host, port, "test_uvclientserver", server.get_loop());
    client.set_bandwidth(1);

    server.run_loop();

    UVBuffers::instance().clear();

    REQUIRE(stdout.data() == "Client       : connected\n"
                             "Client       : sending file test_uvclientserver at 1MBit/s\n"
                             "Server       : client connected\n"
                             "Server client: connected\n"
                             "Server client: disconnected\n"
                             "Server       : client disconnected\n");
}

TEST_CASE("ByteParser")
{
    ByteParser<uint64_t> parser;
    uint64_t integer = 1234567890123456789;
    for (char *ch = reinterpret_cast<char *>(&integer), *end = ch + sizeof(uint64_t); ch < end; ++ch) {
        parser.parse(*ch);
    }
    REQUIRE(parser.value() == integer);
}

TEST_CASE("ArrayParser")
{
    ArrayParser<std::string> parser;
    std::string string = "Test123";
    parser.reset(string.size());
    for (char ch : string) {
        parser.parse(ch);
    }
    REQUIRE(parser.value() == string);
}
