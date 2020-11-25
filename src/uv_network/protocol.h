#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <fstream>
#include "uv_network.h"
#include "parser.h"



class Context;



class State
{
    friend class Context;

public:
    virtual ~State() = default;
    virtual void handle(const char *&/*data*/, const char * const /*end*/) { };
    virtual void handle_init() { };
    virtual bool is_done() const { return false; };

protected:
    Context *m_context = nullptr;
};



class Context
{
public:
    Context(UVAbstractClient *client);

    void switch_to(std::unique_ptr<State> &&state);
    void handle_request(const char *data, const char * const end);
    UVAbstractClient *client() const;
    bool is_done() const;

private:
    std::unique_ptr<State> m_state;
    UVAbstractClient * const m_client;
};



class StateClientSendFile : public State
{
public:
    StateClientSendFile(const std::string &filename, uint64_t bandwidth);
    void handle(const char *&data, const char * const end) override;
    void handle_init() override;

private:
    ByteParser<uint64_t> m_offset;
    std::string m_filename;
    std::ifstream m_file;
    uint64_t m_bandwidth;
};



class StateServerParseInitPacket : public State
{
public:
    void handle(const char *&data, const char * const end) override;

private:
    ByteParser<uint8_t> m_string_size;
    ByteParser<uint64_t> m_filesize;
    ArrayParser<std::string> m_filename;
};



class StateServerReceiveFile : public State
{
public:
    StateServerReceiveFile(std::ofstream &&file, size_t pending_bytes);
    void handle(const char *&data, const char * const end) override;

private:
    std::ofstream m_file;
    size_t m_pending_bytes;
};



class StateDone : public State
{
public:
    void handle(const char *&data, const char * const end) override;
    void handle_init() override;
    bool is_done() const override;
};



class StateClose : public State
{
public:
    void handle(const char *&data, const char * const end) override;
    void handle_init() override;
    bool is_done() const override;
};



#endif // PROTOCOL_H
