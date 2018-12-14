
#include "server.hpp"


#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <event.h>
#include <assert.h>
#include <errno.h>


#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>



#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"



#include "common_stucts.hpp"
#include "iothreadmanager.hpp"
#include "threadex.hpp"



using namespace std;
using namespace muduo;
using namespace muduo::net;

using boost::shared_ptr;



class ServerImpl {
public:
    ServerImpl();
    ~ServerImpl();

    bool serve();
    bool init(TypeProcess tp, TypeProtocol pro, int port, int nthread);
private:
    bool create_and_listen_onsocket();
    bool create_and_bind_socket();
    bool listen_socket();

    bool register_events();
    void handle_event(SOCKET fd, short which);

    static void listen_handler(evutil_socket_t fd, short which, void* v);
private:
    int     m_port;
    SOCKET  m_socket;

    event_thread    * m_thread;
    IOThreadManager * m_tmanager;

    struct event m_listenEvent;
};

ServerImpl::ServerImpl() : m_port(0),
    m_socket(INVALID_SOCKET),
    m_thread(nullptr), m_tmanager(nullptr) {

}

ServerImpl::~ServerImpl() {
    delete_object(m_tmanager);
    delete_object(m_thread);
}

bool ServerImpl::serve() {
    if (!create_and_listen_onsocket()) {
        return false;
    }

    if (!register_events()) {
        return false;
    }

    printf("server start!!!!\n");
    if(!m_thread->start(false)) {
        return false;
    }
    printf("server end\n");
    return true;
}

bool ServerImpl::init(TypeProcess tp, TypeProtocol pro, int port, int nthread) {
    m_port = port;
    LOG_INFO << "listen on: " << port;

    if (m_tmanager == nullptr) {
        m_tmanager = new IOThreadManager(nthread);
    }

    bool ret = m_tmanager->start(tp, pro);
    if (ret == false) {
        return false;
    }

    if (m_thread == nullptr) {
        m_thread = new event_thread(0);
    }
    return true;
}

bool ServerImpl::create_and_listen_onsocket() {
    if (create_and_bind_socket()) {
        if (listen_socket()) {
            return true;
        }
    }
    return false;
}

bool ServerImpl::create_and_bind_socket() {
    struct addrinfo hints, *res, *res0;
    char port[sizeof("65536") + 1];
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    sprintf(port, "%d", m_port);

    int error = getaddrinfo(NULL, port, &hints, &res0);
    if (error) {
        //throw TException("TNonblockingServer::serve() getaddrinfo "
        //                 + string(THRIFT_GAI_STRERROR(error)));
        return false;
    }

    // Pick the ipv6 address first since ipv4 addresses can be mapped
    // into ipv6 space.
    for (res = res0; res; res = res->ai_next) {
        if (res->ai_family == AF_INET6 || res->ai_next == NULL)
            break;
    }

    m_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (m_socket == INVALID_SOCKET) {
        freeaddrinfo(res0);
        //throw TException("TNonblockingServer::serve() socket() -1");
        return false;
    }

#ifdef IPV6_V6ONLY
    if (res->ai_family == AF_INET6) {
        int zero = 0;
        if (-1 == setsockopt(m_socket, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<void*>(&zero), sizeof(zero))) {
            //GlobalOutput("TServerSocket::listen() IPV6_V6ONLY");
        }
    }
#endif // #ifdef IPV6_V6ONLY

    int one = 1;

    // Set THRIFT_NO_SOCKET_CACHING to avoid 2MSL delay on server restart
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<void*>(&one), sizeof(one));

    if (::bind(m_socket, res->ai_addr, static_cast<int>(res->ai_addrlen)) == -1) {
        close(m_socket);
        freeaddrinfo(res0);
        //throw TTransportException(TTransportException::NOT_OPEN,
        //                          "TNonblockingServer::serve() bind",
        //                          THRIFT_GET_SOCKET_ERROR);

        perror("bad bind\n");
        LOG_ERROR << "bind port faild:" << m_port;
        return false;
    }

    // Done with the addr info
    freeaddrinfo(res0);

    // Set up this file descriptor for listening
    return true;
}

bool ServerImpl::listen_socket() {
    // Set socket to nonblocking mode
    int flags;
    if ((flags = fcntl(m_socket, F_GETFL, 0)) < 0
        || fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(m_socket);
        //throw TException("TNonblockingServer::serve() THRIFT_O_NONBLOCK");
        return false;
    }

    int one = 1;
    struct linger ling = {0, 0};

    // Keepalive to ensure full result flushing
    setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<void*>(&one), sizeof(one));

    // Turn linger off to avoid hung sockets
    setsockopt(m_socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<void*>(&ling), sizeof(ling));

    // Set TCP nodelay if available, MAC OS X Hack
    // See http://lists.danga.com/pipermail/memcached/2005-March/001240.html
#ifndef TCP_NOPUSH
    setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<void*>(&one), sizeof(one));
#endif

#ifdef TCP_LOW_MIN_RTO
    if (TSocket::getUseLowMinRto()) {
        setsockopt(s, IPPROTO_TCP, TCP_LOW_MIN_RTO, reinterpret_cast<void*>(&one), sizeof(one));
    }
#endif

    if (listen(m_socket, LISTEN_BACKLOG_AP) == -1) {
        close(m_socket);
        //throw TException("TNonblockingServer::serve() listen");
        return false;
    }

    // Cool, this socket is good to go, set it as the serverSocket_
    //serverSocket_ = s;
    /*
     if (!port_) {
     sockaddr_in addr;
     socklen_t size = sizeof(addr);
     if (!getsockname(serverSocket_, reinterpret_cast<sockaddr*>(&addr), &size)) {
     listenPort_ = ntohs(addr.sin_port);
     } else {
     GlobalOutput.perror("TNonblocking: failed to get listen port: ", THRIFT_GET_SOCKET_ERROR);
     }
     }
     */
    return true;
}

bool ServerImpl::register_events() {
    if (m_thread == NULL || m_socket == INVALID_SOCKET) {
        return false;
    }

    LOG_INFO << "listen srv socket=" << m_socket;
    short flag = m_thread->set_eventcallback(&m_listenEvent, m_socket,
                                             EV_READ | EV_PERSIST,
                                             ServerImpl::listen_handler, this);

    return flag == -1 ? false : true;
}

void OutPeerIpInfo( sockaddr* addrp, int iCliFd ) {
    uint64_t ip   = 0;
    uint16_t port = 0;
    std::string sPeerIp ;

    std::string sType ;
    if (addrp->sa_family == AF_INET) {
        sockaddr_in * ipv4 = (sockaddr_in*)addrp;

        char ip4[INET_ADDRSTRLEN] = { 0 } ;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip4, INET_ADDRSTRLEN);
        sPeerIp = ip4;

        ip   = ipv4->sin_addr.s_addr;
        port = ipv4->sin_port;
        sType= "IpV4";
    } else if (addrp->sa_family == AF_INET6) {
        sockaddr_in6 * ipv6 = (sockaddr_in6*)addrp;

        char ip6[INET6_ADDRSTRLEN] = { 0 } ;
        inet_ntop( AF_INET6, &(ipv6->sin6_addr), ip6, INET6_ADDRSTRLEN );
        sPeerIp = ip6;

        ip   = ipv6->sin6_addr.__in6_u.__u6_addr32[3];
        port = ipv6->sin6_port;
        sType= "IpV6";
    }

    LOG_INFO << sType << ", Client socket: " << iCliFd << ", sPeerIp: " << sPeerIp
             << ", ip: " << ip << ", port: " << ::ntohs(port);
}

void ServerImpl::handle_event(SOCKET fd, short /*which*/) {
    socklen_t addrLen;
    sockaddr_storage addrStorage;
    sockaddr* addrp = (sockaddr*)&addrStorage;
    addrLen = sizeof(addrStorage);

    // Going to accept a new client socket
    SOCKET clientSocket;

    uint64_t ip = 0;
    uint16_t port = 0;

    // Accept as many new clients as possible, even though libevent signaled only
    // one, this helps us to avoid having to go back into the libevent engine so
    // many times
    while ((clientSocket = ::accept(fd, addrp, &addrLen)) != -1) {
        // If we're overloaded, take action here
        ip = 0;
        port = 0;
        if (addrp->sa_family == AF_INET) {
            sockaddr_in * ipv4 = (sockaddr_in*)addrp;
            ip = ipv4->sin_addr.s_addr;
            port = ipv4->sin_port;
        } else if (addrp->sa_family == AF_INET6){
            sockaddr_in6 * ipv6 = (sockaddr_in6*)addrp;
            ip = ipv6->sin6_addr.__in6_u.__u6_addr32[3];
            port = ipv6->sin6_port;
        }

        // Explicitly set this socket to NONBLOCK mode
        int flags;
        if ((flags = fcntl(clientSocket, F_GETFL, 0)) < 0
            || fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) < 0) {
            LOG_ERROR << ", close socket " << clientSocket
                      << ",ip:" << std::hex << ip << ",port:" << std::dec << port;
            close(clientSocket);
            return;
        }

        //printf("we get one %d\n", clientSocket);
        Item_t * item = m_tmanager->get_item();
        assert(item != NULL && "item is NULL");
        item->m_socket = clientSocket;

        if (!m_tmanager->dispatch_item(item)) {
            delete_object(item);
            LOG_ERROR << ", close socket " << clientSocket
                      << ",ip:" << std::hex << ip << ",port:" <<  std::dec << port;
            close(clientSocket);
        }
        OutPeerIpInfo(addrp, clientSocket);
//        LOG_INFO << ", get clientsocket " << clientSocket << ",ip:" << ip << ",port:" << port;
        // Create a new TConnection for this client socket.
        addrLen = sizeof(addrStorage);
    }
    // Done looping accept, now we have to make sure the error is due to
    // blocking. Any other error is a problem
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
        //GlobalOutput.perror("thriftServerEventHandler: accept() ", THRIFT_GET_SOCKET_ERROR);
    }
    //printf("error: %d\n", errno);
}

void ServerImpl::listen_handler(evutil_socket_t fd, short which, void * v) {
    if (v == NULL) {
        return;
    }
    ServerImpl * s = reinterpret_cast<ServerImpl*>(v);
    if (s == NULL) {
        return;
    }
    if (fd != s->m_socket) {
        return;
    }
    s->handle_event(fd, which);
}
/********************************************************************/
/// Server
/********************************************************************/

Server::Server(TypeProcess tp, TypeProtocol pro, int port, int nthread) : m_impl(NULL) {
    init(tp, pro, port, nthread);
}

Server::~Server() {
    delete_object(m_impl);
}

bool Server::serve() {
    if (m_impl == NULL) {
        return false;
    }
    return m_impl->serve();
}

bool Server::init(TypeProcess tp, TypeProtocol pro, int port, int nthread) {
    if (port < 0 || port > 0xffff) {
        return false;
    }
    if (m_impl == NULL) {
        m_impl = new ServerImpl;
        assert(m_impl != NULL && "m_impl is NULL");
    }
    return m_impl->init(tp, pro, port, nthread);
}



