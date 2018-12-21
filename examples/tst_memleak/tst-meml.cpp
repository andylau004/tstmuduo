


#include "tst-meml.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>


#include <list>


#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>



#include <sys/epoll.h>
#include <sys/socket.h>



#include <signal.h>


#include <unordered_set>
#include <boost/circular_buffer.hpp>


#include "muduo/net/Callbacks.h"
#include "muduo/base/common.h"
#include "muduo/base/Logging.h"

#include "muduo/net/EventLoop.h"
#include "muduo/base/Timestamp.h"

#include "muduo/base/libevent.h"

#include "muduo/net/TcpServer.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>


using namespace boost;

using namespace std;
using namespace muduo;
using namespace muduo::net;




int tst_meml_entry(int argc, char *argv[]) {
    int tmp_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    LOG_INFO << "array_size(tmp_array)=" << array_size(tmp_array) * sizeof(int);


    return 1;
}




