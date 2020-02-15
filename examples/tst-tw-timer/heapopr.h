
#pragma once




#include <iostream>
#include <netinet/in.h>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;



#define BUFFER_SIZE 64

struct client_data;


/* 定时器类 */
class heap_timer_node {
public:
    heap_timer_node(int delay) {
//        expire = time(NULL) + delay;
        expire = delay;
        validFlag = true;
    }

public:
    time_t expire;                      /* 定时器生效的绝对时间 */
    void (*cb_func)(client_data *);     /* 定时器的回调函数 */
    client_data *user_data;             /* 用户数据 */
    bool validFlag;                    /* 有效标志 */
};

/* 构建堆时的比较函数 */
bool cmp_heap_timer_node(heap_timer_node *node1, heap_timer_node *node2);


/* 时间堆类 */
class timer_heap {

public:
    timer_heap(int cap);

    void add_timer(heap_timer_node* timer);// 添加目标定时器timer
    void del_timer(heap_timer_node* timer);// 删除目标定时器timer
    heap_timer_node* get_top();// 获得堆顶部的定时器
    void del_top();                         /* 删除堆顶部的定时器 */
    void tick();                            /* 心博函数 */
    void disPlay();                         /* 输出堆中所有定时器 */
    bool is_empty();                        /* 堆是否为空 */

    ~timer_heap();                          /* 销毁时间堆 */

private:
    void resize();                          /* 将当前容量扩大一倍 */

private:
    std::vector<heap_timer_node*> array;    /* 堆数组 */
    int cap;                                /* 堆数组的容量 */
    int cur_size;                           /* 堆数组中当前的元素个数 */
    int del_num;
};


/* 绑定socket和定时器 */
struct client_data {
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    heap_timer_node *timer;
};




extern void tst_heapopr_entry();
