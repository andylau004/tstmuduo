
#pragma once


#include <functional>
#include <list>
#include <thread>
#include <mutex>

#include <boost/function.hpp>



#include <time.h>
#include <netinet/in.h>
#include <stdio.h>


#define BUFFER_SIZE 64


class tw_timer;


struct client_data {

    sockaddr_in address;
    int         client_sock;

    char        buf[BUFFER_SIZE];
    tw_timer*   timerObj;

};


// 定时器
class tw_timer {
public:
    tw_timer(int rot, int ts)
        : next(nullptr), prev(nullptr), rotation(rot), time_solt(ts)
    {
    }

public:
    int rotation;//定时器在时间轮上转多少圈后生效
    int time_solt;//定时器属于时间轮上哪个槽

    void (*cb_func) (client_data*);//定时器的回调函数

    client_data* user_data;//客户端数据

    tw_timer* prev;
    tw_timer* next;

};

// 时间轮
class time_wheel {

public:
    time_wheel();
    ~time_wheel();

    tw_timer* add_timer(int timeout);   //根据定时值创建定时器，并插入合适的位置
    void del_timer(tw_timer *timer);    //删除目标定时器
    void tick();                //时间到后调用该函数，时间轮向前滚动一个槽间隔

private:
    static const int N  = 60;//时间轮上槽的数目
    static const int TI = 1;//槽间隔时间，即每1秒时间轮转动一次

    int cur_slot;                 //时间轮的当前槽
    tw_timer *m_slots[N];         //时间轮的槽，其中每个元素指向一个定时器链表

};






