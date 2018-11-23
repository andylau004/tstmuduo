# tstmuduo

1.    测试 muduo 
2.    测试 实验代码
3.    测试 算法: 堆排序、快排、trie树、二分查找等常用算法
4.    测试 boost库 常用工具类 方法
5.    测试 各种哈希算法，一致性哈希算法
6.    新增 protobuff 测试 序列化/反序列化
7.    新增 测试thrift传二进制文件，同时，新增thrift server/client 封装工具类，                   
                   
8.    新增 boost::shared_ptr<TNonblockingIOThread>  m_listenThread;
      此线程只负责侦听接受客户端连接
```
    // 主侦听线程，只负责侦听客户端socket，push新连接到io thread
    boost::shared_ptr<TNonblockingIOThread>  m_listenThread;

    // Register the events for the primary (listener) IO thread
    // ioThreads_[0]->register_io_events();
    m_listenThread->register_io_events();
```

9.    新增    
class ExProcessorEventHandler: public apache::thrift::TProcessorEventHandler
      根据 getContext  和 postWrite 之间 调用时间差，打印指定接口--函数的调用耗时


10.   不修改thrift源码获取thrift客户端IP，通过注入
```
class ExServerEventHandler: public apache::thrift::server::TServerEventHandler
```

11.   新增　tstc11 测试用例
      测试　ON_SCOPE_EXIT　C++11 实现 golang defer　延迟释放

12.   新增　tstimewheel
      测试　时间轮　算法

13.   新增　模仿muduo内timewheel实现方式，实现时间轮
      ./examples/tstbucketwheel/tstbucketwheel.cpp

14.   新增　在修改过的thrift中实现时间轮，用来监控连接是否在指定时间内是否有数据交互，如果没有则析构连接
      ./examples/transferserver/iothread.cpp
      测试通过





计划：

1.    下一步考虑 taskthread 增加多线程，每个线程一个队列

2.    计划新增 thrift 0.9.3 自修改版本，修复其中的bug，以及尝试提升性能patch






