
#include "rdfun.h"


#include <stdio.h>
#include <sstream>
#include <ostream>


#include <signal.h>
#include <sys/time.h>

#include <stdio.h>

#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


//#include "poco/Bugcheck.h"
//#include "poco/Foundation.h"
//#include "poco/Alignment.h"

#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "shm_queue.h"
#include "muduo/base/shared_mem.h"



using namespace std;
using namespace muduo;
using namespace muduo::net;

#include <sys/shm.h>



void siguser1(int signo) // dummy signal handler
{
    (void)signo;
    printf("acept a sig, signo=%d\n", signo);
}



void tst_rd_2() {
    bool bexist = SharedMem::IsShmExist(SHM_KEY);
    LOG_INFO << "sharemem exist = " << bexist;

}


void tst_rd_mem() {
    tst_rd_2();
    return;


    long shmkey = 0x12345;
    struct sq_head_t* sq = sq_open(shmkey);
    if (!sq) {
        LOG_ERROR << "sq create share memory failed!!!";
        return;
    }
    signal(SIGUSR1, siguser1);// 如果需要signal通知，需要向系统注册一个signal handler

    // 向队列注册我们的pid以便接收通知
    // 如果你的进程需要fork多个进程，一定好保证在sq_register_signal()调用之前进行
    int sigindex = sq_register_signal(sq);
//    LOG_INFO << "AAAAAAAAAAAAAAAAAAAA";

    while (1) {
        char buffer[ 100 * 1024 ] = {0};
        struct timeval write_time;

        int len = sq_get(sq, buffer, sizeof(buffer), &write_time);
        if(len<0) // 读失败
        {
            LOG_ERROR << "sq_get error!!! len=" << len;
        }
        else if(len==0) // 没有数据，继续做其它操作，然后等待，这里可以进入select/epoll_wait等待
        {
            LOG_INFO << "len == 0";
            sq_sigon(sq, sigindex); // 打开signal通知
            sleep(1);  //可以进入select 或者 epoll_wait 等待
            sq_sigoff(sq, sigindex); // 关闭signal通知
        }
        else // 收到数据了
        {
            printf("sec %ds,usec%dus I am reader: %s\n",write_time.tv_sec,write_time.tv_usec,buffer);
        }
    }
}

void tst_rd1() {

    void* shm = nullptr;
    struct shared_use_st* sharedMem = nullptr;
    int shmId = 0;

    shmId = ::shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if (shmId == -1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    shm = ::shmat(shmId, 0, 0);
    if (shm == (void*)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n", shm);

    sharedMem = (struct shared_use_st*)shm;
    sharedMem->written = 0;

    while (1) {

        if (sharedMem->written == 1) {
            printf( "someone wrote content=%s\n", sharedMem->text );

            // 读取完数据，设置written使共享内存段可写
            sharedMem->written = 0;

            if (strncasecmp(sharedMem->text, "end", 3) == 0) {
                break;
            }
        } else {
            ::usleep( 1000 * 200 );
        }

    }
    // 把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    // 删除共享内存
    if (shmctl(shmId, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}



