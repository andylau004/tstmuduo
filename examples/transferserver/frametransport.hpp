#pragma once


#include <stdio.h>

#include <boost/scoped_ptr.hpp>

#include "transport.hpp"

enum SocketState {
    SOCKET_RECV_FRAMING,
    SOCKET_RECV,
    SOCKET_SEND
};

enum AppState {
    APP_INIT,
    APP_READ_FRAME_SIZE,
    APP_READ_REQUEST,
    APP_WAIT_TASK,
    APP_SEND_RESULT,
    APP_CLOSE_CONNECTION
};

enum ServerState {
    SERVER_READ,
    SERVER_WRITE
};

union Framing {
    uint8_t buf[sizeof(uint32_t)];
    uint32_t size;
};

#define FRAME_SIZE sizeof(uint32_t)


class FrameTransport : public Transport {
public:
    FrameTransport(uint32_t readBufferSize, uint32_t writeBufferSize);
    explicit FrameTransport(uint32_t size);

    virtual ~FrameTransport();

    virtual bool init();

    virtual void work_socket(int sfd);
    virtual void write_socket(int sfd);

    virtual void reset_write_state();

private:
    bool read_framing();
    bool read_data();
    bool write_data();

    void transition();

private:
    boost::scoped_ptr<Framing> m_framing;
//    Framing     *   m_framing;
    SocketState     m_socketState;
    AppState        m_appState;
    ServerState     m_serverState;
    uint32_t        m_readWant;
};

