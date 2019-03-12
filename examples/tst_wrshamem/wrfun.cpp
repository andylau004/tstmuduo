
#include "wrfun.h"


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




void siguser1(int signo) // dummy signal handler
{
    (void)signo;
    printf("acept a sig, signo=%d\n", signo);
}

void tst_wr_2() {
    bool bexist = SharedMem::IsShmExist(SHM_KEY);
    LOG_INFO << "sharemem exist = " << bexist;

}

void tst_wr_mem() {
    tst_wr_2();
    return ;


    long shmkey = 0x12345;

    int element_size  = 64; // 基本块的大小，如果数据小于1个块，就按1个块存储，否则，存储到连续多个块中，只有第一个块有块头部信息
    int element_count = 1024; // 队列的长度，总共有多少个块

//    struct sq_head_t *sq = sq_open(shmkey);
    struct sq_head_t *sq = sq_create(shmkey, element_size, element_count);
    if (!sq) {
        LOG_INFO << "create sq obj failed";
        return ;
    }

    // 如果需要开启signal通知功能，设置一下通知的参数
    sq_set_sigparam(sq, SIGUSR1, 1, 2);

    // 现在可以开始写数据了
    char *data = "hello hello I am writer";
    int iret = sq_put(sq, data, strlen(data));
    LOG_INFO << "qut iret=" << iret;
    if ( iret < 0 )
    {
        // 队列满了。。。
        LOG_INFO << "queue is full";
    }

    getchar ();
    sq_destroy(sq);
//    return 0;
}

void tst_wr1() {

    void* shm = nullptr;
    struct shared_use_st* shared_mem = nullptr;
    char buffer[ BUFSIZ + 1 ] = {0};
    int shmId = 0;

    shmId = ::shmget( (key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT );
    if (shmId == -1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    shm = ::shmat( shmId, 0, 0 );
    if (shm == (void*)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attched at %X\n", shm);

    shared_mem = (struct shared_use_st*) shm;
    while (1) {

        if (shared_mem->written == 1) {
            ::sleep(1);
            printf( "some one is writting...\n" );
        }
        // 向共享内存中写入数据
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared_mem->text, buffer, TEXT_SZ);

        shared_mem->written = 1;
        // 输入了end，退出循环（程序）
        if (strncmp(buffer, "end", 3) == 0)
        {
            break;
        }

    }

    // 把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(2);
    exit(EXIT_SUCCESS);
}



