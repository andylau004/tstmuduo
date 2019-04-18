
#include "frametransport.hpp"


#include <assert.h>
#include <arpa/inet.h>

#include "muduo/base/common.h"
#include "muduo/base/Logging.h"

//#include "logs/log.h"
//#include "base/hollyalloc.h"

//extern log4cplus::Logger gLogger;

#define MAX_FRAME_SIZE_AP 1024 * 1024 // 1M


FrameTransport::FrameTransport(uint32_t readBufferSize,
                               uint32_t writeBufferSize) :
    Transport(readBufferSize, writeBufferSize),
    m_framing(NULL),
    m_socketState(SOCKET_RECV_FRAMING),
    m_appState(APP_INIT),
    m_serverState(SERVER_READ),
    m_readWant(0) {
}

FrameTransport::FrameTransport(uint32_t size) :
    Transport(size),
    m_framing(NULL),
    m_socketState(SOCKET_RECV_FRAMING),
    m_appState(APP_INIT),
    m_serverState(SERVER_READ),
    m_readWant(0) {
}

FrameTransport::~FrameTransport() {
//    delete_object(m_framing);
}

bool FrameTransport::init() {
    if (!Transport::init()) {
        return false;
    }
//    if (m_framing == nullptr) {
//        m_framing = new (std::nothrow) Framing;
//        if (m_framing == nullptr) {
//            LOG_ERROR << " new m_framing error ";
//            return false;
//        }
//    }
    if (!m_framing) {
        m_framing.reset(new Framing);
    }
    m_socketState = SOCKET_RECV_FRAMING;
    m_appState    = APP_INIT;
    m_serverState = SERVER_READ;
    m_readWant    = 0;
    return true;
}

void FrameTransport::work_socket(int sfd) {
    if (m_socket == INVALID_SOCKET && m_appState == APP_INIT) {
        m_socket = sfd;
    } else {
        if (m_socket != sfd) {
            SignalCloseSocket();
            return;
        }
    }

    switch (m_socketState) {
        case SOCKET_RECV_FRAMING:
            if (m_serverState == SERVER_WRITE) {
                // the client does not follow the rules
                LOG_ERROR << ", client does not follow the rules";
                SignalClientCloseSocket();
                return;
            }
            LOG_DEBUG << ", RECV_FRAMING sd " << sfd;
            if (read_framing()) {
                transition();
            }
            break;
        case SOCKET_RECV:
            if (m_serverState == SERVER_WRITE) {
                // the client does not follow the rules
                LOG_ERROR << ", client does not follow the rules";
                SignalClientCloseSocket();
                return;
            }
            LOG_DEBUG << ", RECV sd " << sfd;
            if (read_data()) {
                transition();
            }
            break;
        case SOCKET_SEND:
            LOG_INFO << ", SEND sd " << sfd;
            if (write_data()) {
                transition();
            }
            break;
        default:
            LOG_ERROR << ", unknown state,  DEFAULT sd " << sfd;
            SignalCloseSocket();
            break;
    }

}

void FrameTransport::write_socket(int sfd) {
    if (m_socket != sfd) {
        SignalCloseSocket();
        return;
    }
    transition();
}

/// TODO(H.haiwei) 如果作为一个单独的服务器,这里可能需要保护
/// 如果隐藏在NGINX后面的话,这里可以不用考虑
bool FrameTransport::read_framing() {
    assert(m_framing != NULL);

    m_framing->size = m_readWant;
    int fetch = read(&m_framing->buf[m_readBufferPos], FRAME_SIZE - m_readBufferPos);
    if (fetch <= 0) {
        LOG_ERROR << ",error read, fetch=" << fetch << ", m_readBufferPos=" << m_readBufferPos << " size=" << FRAME_SIZE - m_readBufferPos;
        SignalCloseSocket();
        return false;
    }
    m_readBufferPos += fetch;
    if (m_readBufferPos < FRAME_SIZE) {
        m_readWant = m_framing->size;
        // 这里要小心, 因为, 如果对方只发了一个字节的数据呢?
        LOG_WARN << ", need reading";
        return false;
    }

    // when we get here, it means we have got the hole header
    // header is right, will read the body
    m_readWant = ntohl(m_framing->size);

    if (m_readWant > MAX_FRAME_SIZE_AP) {
        LOG_ERROR << ", m_readWant:" << m_readWant;
        SignalCloseSocket();
        return false;
    }
    m_appState = APP_READ_FRAME_SIZE;

    // 触发时间轮 -- 该连接收到数据
//    SingalRecvSomeData();
    return true;
}

void FrameTransport::transition() {

    switch(m_appState) {
        case APP_READ_FRAME_SIZE:
            LOG_INFO << ",READ_FRAME_SIZE m_readWant:" << m_readWant
                     << ",m_readBufferSize:" << m_readBufferSize;
            if (m_readWant == 0) {
                SignalCloseSocket();
                return;
            }
            if (m_readWant > m_readBufferPos) {
                if (m_readBufferSize == 0) m_readBufferSize = 2;
                uint32_t newSize = m_readBufferSize;
                while (m_readWant > newSize)
                    newSize *= 2;
                uint8_t* newbuffer = (uint8_t*)realloc(m_readBuffer, newSize);
                if (newbuffer == nullptr) {
                    LOG_ERROR << ", alloc error, size:" << newSize;
                    SignalCloseSocket();
                    return;
                }
                m_readBuffer     = newbuffer;
                m_readBufferSize = newSize;
            } else {
                // tcmalloc will hold the performance
                if (m_readBufferSize > m_readWant * 2) {
                    size_t size = holly_align(m_readWant, 8);
                    if (size >= m_readWant) {
                        uint8_t * newBuffer = (uint8_t*)malloc(size);
                        if (newBuffer) {
                            free(m_readBuffer);
                            m_readBuffer = newBuffer;
                            m_readBufferSize = size;
                        }
                    }
                }
            }
            m_readBufferPos = 0;
            m_socketState   = SOCKET_RECV;
            m_appState      = APP_READ_REQUEST;
            return;

        case APP_READ_REQUEST: // 解析body
            LOG_INFO << ", READ_REQUEST read done";
            m_inMemBuffer->resetBuffer(m_readBuffer, m_readWant);
            m_outMemBuffer->resetBuffer();
            m_outMemBuffer->getWritePtr(4);
            m_outMemBuffer->wroteBytes(4);
            m_appState    = APP_WAIT_TASK;
            m_serverState = SERVER_WRITE;
            SignalReadSocketDone();
            break;

        case APP_WAIT_TASK:
            m_outMemBuffer->getBuffer(&m_writeBuffer, &m_writeBufferSize);
            if (m_writeBufferSize > 4) {
                m_writeBufferPos = 0;
                m_socketState    = SOCKET_SEND;

                uint32_t frameSize = (uint32_t)::htonl(m_writeBufferSize - 4);
                memcpy(m_writeBuffer, &frameSize, 4);
                m_appState = APP_SEND_RESULT;
                SignalSetWriteState();
                return;
            }
            goto LABEL_APP_INIT;

        case APP_SEND_RESULT: // 目前并不做什么
        LABEL_APP_INIT:
        case APP_INIT:
                LOG_INFO << ", INIT, set read";
                m_writeBuffer     = NULL;
                m_writeBufferPos  = 0;
                m_writeBufferSize = 0;

                m_socketState     = SOCKET_RECV_FRAMING;
                m_appState        = APP_READ_FRAME_SIZE;
                m_serverState     = SERVER_READ;
                m_readBufferPos   = 0;

                //SignalSetWriteState();
                SignalSetReadState();
                return;
        default:
                LOG_ERROR << ", bad state ";
                SignalCloseSocket();
                break;
    }

}

bool FrameTransport::read_data() {
    uint32_t fetch = m_readWant - m_readBufferPos;
    if (fetch <= 0) {
        SignalCloseSocket();
        LOG_ERROR << ", m_readWant:" << m_readWant << ",m_readBufferPos:" << m_readBufferPos;
        return false;
    }
    int got = read(m_readBuffer + m_readBufferPos, fetch);
    if (got > 0) {
        m_readBufferPos += got;
        if (m_readBufferPos == m_readWant) {
            LOG_INFO << ", success, m_readWant:" << m_readWant;
            return true;
        }
        // 这个地方需要确保数据是准确的,否则, 恶意的减少字节会导致 该链接被无辜的占用
        return false;
    }
    // whenever we got here, it means something has wrong
    LOG_ERROR << ", m_readWant:" << m_readWant << ", m_readBufferPos:" << m_readBufferPos;
    SignalCloseSocket();
    return false;
}

bool FrameTransport::write_data() {
    if (m_writeBufferPos == m_writeBufferSize) {
        LOG_INFO << ", success, m_writeBufferSize:" << m_writeBufferSize;
        return true;
    }

    int left = m_writeBufferSize - m_writeBufferPos;
    int sent = write(m_writeBuffer + m_writeBufferPos, left);
    if (sent < 0) {
        LOG_ERROR << ", error, m_writeBufferSize:" << m_writeBufferSize;
        SignalCloseSocket();
        return false;
    }
    m_writeBufferPos += sent;

    if (m_writeBufferPos == m_writeBufferSize) {
        LOG_INFO << ", success, m_writeBufferSize:" << m_writeBufferSize;
        return true;
    }
    return false;
}

void FrameTransport::reset_write_state() {
    m_appState = APP_WAIT_TASK;
    m_serverState = SERVER_WRITE;
}

