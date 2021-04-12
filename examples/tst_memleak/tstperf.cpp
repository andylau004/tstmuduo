

#include "tstperf.h"

#include <stdio.h>
#include <stdlib.h>
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


// for memory access forbidden
#include <execinfo.h>
#include <unistd.h>
#include <signal.h>
// ---------------------------




struct PrimeNumberNode
{
    int prime;
    int count;
    struct PrimeNumberNode *next;
};

int wasteTime(int factor)
{
    int i = 0;
    int testResult;
    for (i = 0; i < factor; i++)
    {
        testResult += i;
        testResult = testResult * 1;
    }
    return testResult;
}

//  function to get prime number which is very hot
struct PrimeNumberNode *number(int input)
{
    int i;
    int j;

    struct PrimeNumberNode *head = (struct PrimeNumberNode *)malloc(sizeof(struct PrimeNumberNode));
    struct PrimeNumberNode *currentNode = head;
    for (i = 3; i < input; i++)
    {
        if (i % 2 == 1)
        {
            for (j = 2; j < i; j++)
            {
                int testResult = wasteTime(j);
                // if (testResult % 10000 == 9999)
                //     printf("Test code");
                if (i % j == 0)
                    continue;
            }
            struct PrimeNumberNode *primeNode = (struct PrimeNumberNode *)malloc(sizeof(struct PrimeNumberNode));
            currentNode->next = primeNode;
            currentNode = primeNode;
            head->count++;
        }
    }

    return head;
}

void tst_perf_1(int argc, char *argv[]) {

    auto numParam = atoi(argv[1]);
    printf("input=%d\n", numParam);

    struct PrimeNumberNode* head = number( numParam );
    printf("count1=%d\n",head->count);

    head = number( numParam );
    printf("count2=%d\n",head->count);

}// perf record -F 999 ./tstmemleak 2000
