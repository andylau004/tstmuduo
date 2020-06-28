

#include "branch_predict.h"


#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <functional>

//#include <boost/circular_buffer.hpp>

//#include "muduo/base/common.h"

//#include <boost/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/scoped_ptr.hpp>

//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/thread/thread.hpp>


//#include "muduo/net/InetAddress.h"
//#include "muduo/net/Channel.h"
//#include "muduo/net/EventLoop.h"
//#include "muduo/net/EventLoopThread.h"
//#include "muduo/net/EventLoopThreadPool.h"
//#include "muduo/net/TcpServer.h"
//#include "muduo/base/Logging.h"
//#include "muduo/net/TcpClient.h"
//#include "muduo/base/ThreadPool.h"



using namespace std;

#define TESTN 128*1024*1024L



long timediff(clock_t t1, clock_t t2) {
    //使用clock统计与取系统时间不同，它表示从进程启动到当下所消耗的CPU计时单元数
    long elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

// 2020-6-20
// add new 测试分支预测
void tstBranchPredict(int argc, char *argv[]) {
    int mode = 1;
    int ch;

    while ((ch = getopt(argc, argv, "fsg")) != -1) {
        switch (ch)
        {
        case 's': //遍历随机数组
            mode = 1;
            break;
        case 'f': //遍历有序数组
            mode = 2;
            break;
        case 'g': //生成数组至文件中，不影响遍历过程时的perf统计
            mode = 3;
            break;
        }
    }

    unsigned char* arr = new unsigned char[TESTN];
    if (3 == mode) {
        for (int i = 0; i < TESTN; i ++ ) arr[i] = rand() % 256;
        ofstream ofs;
        ofs.open("rand.array", ios::binary|ios::out);
        ofs.write((const char*)arr, TESTN);
        ofs.close();
        //数组排序
        std::sort(arr, arr+TESTN);
        ofs.open("sort.array", ios::binary|ios::out);
        ofs.write((const char*)arr, TESTN);
        ofs.close();
    } else {
        const char* fname;
        if (1 == mode) {
            fname = "rand.array";
        }else if (2 == mode) {
            fname = "sort.array";
        }
        ifstream ifs;
        ifs.open(fname);
        ifs.read((char *)arr, TESTN);

        //使用clock比取系统时间能够更准确的看到消耗了多少CPU资源
        clock_t start, end;
        start = clock();
        for(long i = 0; i < TESTN; i++) {
            //条件分支预测在这里发生作用
            if (arr[i] < 128) arr[i] = 0;
        }
        end = clock();
        std::cout << " " << timediff(start, end) << std::endl;
    }
}



