


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


#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>



/*
 * 安装mtrace
 * sudo yum install glibc-utils
 *
三、运行可执行文件
./tst_meml
四、使用perl脚本mtrace将机器指令装换成易读的输出
[root@cf07b5394847 build]# mtrace ./tst_meml mtrace.log

- 0x0000000000968110 Free 3 was never alloc'd 0x7f12ef4dd24d
- 0x0000000000968130 Free 4 was never alloc'd 0x7f12ef536e8c
- 0x00000000009680f0 Free 5 was never alloc'd 0x7f12ef53706f
- 0x00000000009680d0 Free 6 was never alloc'd 0x7f12ef537087
- 0x0000000000968330 Free 7 was never alloc'd 0x7f12ef53703c
- 0x0000000000968070 Free 8 was never alloc'd /usr/local/include/boost/exception/exception.hpp:486
- 0x00000000009680b0 Free 9 was never alloc'd /usr/local/include/boost/smart_ptr/detail/sp_counted_impl.hpp:53
- 0x0000000000968010 Free 10 was never alloc'd /usr/local/include/boost/exception/exception.hpp:486
- 0x0000000000968050 Free 11 was never alloc'd /usr/local/include/boost/smart_ptr/detail/sp_counted_impl.hpp:53

Memory not freed:
-----------------
           Address     Size     Caller
0x0000000000968160     0x14  at /host_WorkDir/mygitwork/c++work/new-tstmuduo/examples/tst_memleak/tst-meml.cpp:69

 * */



/*
[root@cf07b5394847 build]# valgrind --leak-check=full  ./tst_meml
==14194== Memcheck, a memory error detector
==14194== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==14194== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==14194== Command: ./tst_meml
==14194==
20181221 04:32:19.734241Z 14194 INFO  pid = 14194, tid = 14194 - main.cpp:53
20181221 04:32:19.768142Z 14194 INFO  array_size(tmp_array)=40 - tst-meml.cpp:76
==14194==
==14194== HEAP SUMMARY:
==14194==     in use at exit: 1,100 bytes in 3 blocks
==14194==   total heap usage: 13 allocs, 10 frees, 1,948 bytes allocated
==14194==
==14194== 20 bytes in 1 blocks are definitely lost in loss record 1 of 3
==14194==    at 0x4C291BA: malloc (vg_replace_malloc.c:298)
==14194==    by 0x416D50: tst_mem_leak_() (tst-meml.cpp:69)
==14194==    by 0x416E75: tst_meml_entry(int, char**) (tst-meml.cpp:78)
==14194==    by 0x41883F: main (main.cpp:55)
==14194==
==14194== LEAK SUMMARY:
==14194==    definitely lost: 20 bytes in 1 blocks
==14194==    indirectly lost: 0 bytes in 0 blocks
==14194==      possibly lost: 0 bytes in 0 blocks
==14194==    still reachable: 1,080 bytes in 2 blocks
==14194==         suppressed: 0 bytes in 0 blocks
==14194== Reachable blocks (those to which a pointer was found) are not shown.
==14194== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==14194==
==14194== For counts of detected and suppressed errors, rerun with: -v
==14194== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 4 from 4)
*/

void tst_mem_leak_() {

    while (1 ) {

        char* pMem = new char[ 8 * 1024 ];

        sleep( 1 );
    }

    return ;

    setenv("MALLOC_TRACE", "mtrace.log", 1);

    char *hello;
    mtrace();
    hello = (char*) malloc(20);
    sprintf(hello,"nhello world!");
}

int tst_meml_entry(int argc, char *argv[]) {
    int tmp_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    LOG_INFO << "array_size(tmp_array)=" << array_size(tmp_array) * sizeof(int);

    tst_mem_leak_();
    return 1;
}




