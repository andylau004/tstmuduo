
#include "transport.hpp"




#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <errno.h>

//#include "logs/log.h"
#include "muduo/base/common.h"
#include "muduo/base/Logging.h"

//extern log4cplus::Logger gLogger;

Transport::Transport(uint32_t readBufferSize, uint32_t writeBufferSize) :
    m_readBuffer(NULL),
    m_writeBuffer(NULL),
    m_readBufferSize(readBufferSize),
    m_writeBufferSize(writeBufferSize),
    m_readBufferPos(0),
    m_writeBufferPos(0),
    m_socket(INVALID_SOCKET) {
    m_outputTransport.reset(new transport::TMemoryBuffer(writeBufferSize));
}

Transport::Transport(uint32_t size) :
    m_readBuffer(NULL),
    m_writeBuffer(NULL),
    m_readBufferSize(size),
    m_writeBufferSize(size),
    m_readBufferPos(0),
    m_writeBufferPos(0),
    m_socket(INVALID_SOCKET) {
    m_outputTransport.reset(new transport::TMemoryBuffer(size));
}

Transport::~Transport() {
    if (m_readBuffer) {
        free(m_readBuffer);
    }
}

bool Transport::init() {
    if (m_readBuffer == NULL) {
        m_readBuffer = (uint8_t*)malloc(sizeof(uint8_t) * m_readBufferSize);
        if (m_readBuffer == NULL) {
            return false;
        }
        m_inputTransport.reset(new transport::TMemoryBuffer(m_readBuffer, sizeof(uint8_t) * m_readBufferSize));
    }

    m_writeBuffer = NULL;
    m_writeBufferSize = 0;

    m_readBufferPos = 0;
    m_writeBufferPos = 0;
    m_socket = INVALID_SOCKET;
    return true;
}


void Transport::work_socket(int /*sfd*/) {}

void Transport::write_socket(int /*sfd*/) {}

int32_t Transport::read(uint8_t * buf, uint32_t len) {
    if (m_socket == INVALID_SOCKET || buf == NULL || len == 0) {
        LOG_ERROR << ", sd:" << m_socket << ", len:" << len;
        return -1;
    }
    int8_t retries = 0;
    int got = 0;
    while (true) {
        got = static_cast<int>(recv(m_socket, (void*)buf, len, 0));
        if (got < 0) {
            if (errno == ERR_EAGAIN || errno == ERR_EWOULDBLOCK || errno == ERR_EINTR) {
                //
                if (retries++ < 5) {
                    continue;
                }
                LOG_WARN << " fd:" << m_socket << ", err:" << errno;
            }
            // 保留 , 处理下错误
            if (errno == ERR_ECONNRESET) {
                break;
            } else if (errno == ERR_ENOTCONN) {
                break;
            } else if (errno == ERR_ETIMEDOUT) {
                break;
            } else {
                break;
            }
        }
        break;
    }
    LOG_INFO << " fd:" << m_socket << ", got:" << got;
    return got;
}

int32_t Transport::write(const uint8_t * buf, uint32_t len) {
    if (buf == NULL || len == 0 || m_socket == INVALID_SOCKET) {
        LOG_ERROR << ", sd:" << m_socket << ", len:" << len;
        return -1;
    }

    int32_t sent = 0;
    int flags = 0;
#ifdef MSG_NOSIGNAL
    flags |= MSG_NOSIGNAL;
#endif

    int b = static_cast<int>(send(m_socket, (void*)(buf+sent), len - sent, flags));
    if (b < 0) {
        LOG_ERROR << ", sd:" << m_socket << ",err:" << errno;
        if (errno == ERR_EWOULDBLOCK || errno == ERR_EAGAIN) {
            return 0;
        }
        if (errno == ERR_EPIPE || errno == ERR_ECONNRESET || errno == ERR_ENOTCONN) {
            return -1;
        }
        return -1;
    }
    if (b == 0) {
        return -1;
    }
    return b;
}

void Transport::reset_write_state() {

}

