#include "protocol.h"
#include <thread>
#include <chrono>
#include <filesystem>
#include <iostream>
#include "uv_buffers.h"



Context::Context(UVAbstractClient *client) :
    m_state(std::make_unique<State>()), m_client(client)
{

}

void Context::switch_to(std::unique_ptr<State> &&state)
{
    (m_state = std::move(state))->m_context = this;
    m_state->handle_init();
}

void Context::handle_request(const char *data, const char * const end)
{
    while (m_state && data != end) {
        m_state->handle(data, end);
    }
}

UVAbstractClient *Context::client() const
{
    return m_client;
}

bool Context::is_done() const
{
    return m_state->is_done();
}



void StateClose::handle(const char *&data, const char * const end)
{
    data = end;
}

void StateClose::handle_init()
{
    m_context->client()->close();
}

bool StateClose::is_done() const
{
    return true;
}



void StateDone::handle(const char *&data, const char * const end)
{
    data = end;
}

void StateDone::handle_init()
{
//    m_context->client()->close();
}

bool StateDone::is_done() const
{
    return true;
}



StateClientSendFile::StateClientSendFile(const std::string &filename, uint64_t bandwidth) :
    m_filename(filename), m_file(filename, std::ios_base::in | std::ios_base::binary), m_bandwidth(bandwidth * 1000 * 1000)
{

}

void StateClientSendFile::handle(const char *&data, const char * const end)
{
    using namespace std::chrono;

    while (data != end) {
        if (!m_offset.is_valid() && m_offset.parse(*data++)) {
            m_file.seekg(m_offset.value(), std::ios::beg);

            uint64_t uploaded = 0;
            const auto ts = steady_clock::now();

            while (!m_file.eof()) {
                auto buf = m_context->client()->get_buffer();
                m_file.read(buf.base, buf.len);
                const auto len = m_file.gcount();
                if (len <= 0) {
                    UVBuffers::instance().store(std::move(buf));
                    break;
                }
                m_context->client()->write(std::move(buf), len);

                if (m_bandwidth) {
                    uploaded += len;
                    const auto elapsed = duration_cast<milliseconds>(steady_clock::now() - ts).count();
                    const uint64_t speed = 8 * uploaded * 1000 / (elapsed ? elapsed : 1);
                    if (speed > m_bandwidth) {
                        std::this_thread::sleep_for(std::chrono::milliseconds((speed - m_bandwidth) * 1000 / m_bandwidth));
                    }
                }
            }
            m_context->switch_to(std::make_unique<StateDone>());
        }
    }
}

void StateClientSendFile::handle_init()
{
    auto buf = m_context->client()->get_buffer();
    char *ptr = buf.base;

    uint8_t filename_size = m_filename.size();
    memcpy(ptr, &filename_size, sizeof(filename_size));
    ptr += sizeof(filename_size);

    m_file.seekg(0, std::ios::end);
    uint64_t file_size = m_file.tellg();
    memcpy(ptr, &file_size, sizeof(file_size));
    ptr += sizeof(file_size);

    memcpy(ptr, m_filename.data(), m_filename.size());
    ptr += filename_size;

    m_context->client()->write(std::move(buf), ptr - buf.base);
}



void StateServerParseInitPacket::handle(const char *&data, const char * const end)
{
    while (data != end) {
        if (!m_string_size.is_valid()) {
            if (m_string_size.parse(*data++)) {
                m_filename.reset(m_string_size.value());
            }
        }
        else if (!m_filesize.is_valid()) {
            m_filesize.parse(*data++);
        }
        else if (!m_filename.is_valid()) {
            if (m_filename.parse(*data++)) {
                std::ofstream file(std::filesystem::path(m_filename.value()).filename().string(), std::ios_base::binary | std::ios_base::app);
                const uint64_t offset = file.tellp();
                auto buf = m_context->client()->get_buffer();
                char *ptr = buf.base;
                memcpy(ptr, &offset, sizeof(offset));
                ptr += sizeof(offset);
                m_context->client()->write(std::move(buf), ptr - buf.base);
                if (offset == m_filesize.value()) {
                    m_context->switch_to(std::make_unique<StateClose>());
                    return;
                }
                m_context->switch_to(std::make_unique<StateServerReceiveFile>(std::move(file), m_filesize.value() - offset));
                return;
            }
        }
    }
}



StateServerReceiveFile::StateServerReceiveFile(std::ofstream &&file, size_t pending_bytes) :
    m_file(std::move(file)), m_pending_bytes(pending_bytes)
{

}

void StateServerReceiveFile::handle(const char *&data, const char * const end)
{
    if (data != end) {
        const auto size = std::min<ssize_t>(m_pending_bytes, end - data);
        if (size > 0) {
            m_file << std::string_view(data, size);
            data += size;
            m_pending_bytes -= size;
        }
        if (!m_pending_bytes) {
            m_file.flush();
            m_context->switch_to(std::make_unique<StateClose>());
        }
    }
}
