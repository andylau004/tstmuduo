#pragma once




#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread, std::this_thread::yield


#include <cstdio>
#include <cstdlib>
#include <functional>

#include <atomic>
#include <unordered_set>

#include <vector>


#include <boost/circular_buffer.hpp>

#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>


#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "muduo/base/ThreadPool.h"


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>



class DataStructure {
public:
    DataStructure() {
        countAdd=0;
        countDelete=0;
        countFind=0;
    }
    virtual ~DataStructure() {}

    void resetCountAdd() {
        countAdd=0;
    }
    void resetCountFind() {
        countFind=0;
    }
    void resetCountDelete() {
        countDelete=0;
    }

    unsigned int getCountAdd() {
        return countAdd;
    }
    unsigned int getCountDelete() {
        return countDelete;
    }
    unsigned int getCountFind() {
        return countFind;
    }

protected:
    unsigned int countAdd;
    unsigned int countDelete;
    unsigned int countFind;
};

const int keyLength = 128;

class Key : public string {
public:
    Key():string(keyLength, ' ') {}
    Key(const char in[]): string(in) {}
    Key(const string& in): string(in) {}

    virtual ~Key() {}
};

class SkipListNode;
class SkipListNode: public Key {
public:
    SkipListNode(const Key& key, int height):Key(key),m_links(height) {
        assert (height>0);
        int i;
        for (i=0; i<height; i++) {
            m_links[i] = NULL;
        }
    }
    virtual ~SkipListNode() {}


    //retuns the hight of this node
    unsigned int height() {
        return m_links.size();
    };

    //returns pointer to the next node at a given level
    SkipListNode* nextAtLevel(unsigned int level) {
        assert (level >=0 && level < m_links.size());
        return m_links[level];
    };

    //setup the pointer at the next node at a given level
    void setNextAtLevel(unsigned int level, SkipListNode* next) {
        assert (level >=0 && level < m_links.size());
        m_links[level] = next;
    }

private:
    SkipListNode() {}
    std::vector<SkipListNode*> m_links;
};



class SkipList : public DataStructure {
public:
    SkipList() {}
    SkipList(int maxHeight) {
        init(maxHeight);
    }
    virtual ~SkipList();
    void init(int maxHeight);


    // ADD FUNCTIONS
    int add(const Key& key, bool verbose=false);
    unsigned int randHeight();
    int add(SkipListNode* target, SkipListNode* newNode, unsigned int level);

    // FIND FUNCTIONS
    int find(const Key &key, bool verbose=false);
    SkipListNode* find(SkipListNode* target, const Key& key, unsigned int level);

    //DEL FUNCTIONS
    int del(const Key& key, bool verbose= false);
    SkipListNode* del(SkipListNode* target, const Key& key, unsigned int level);

    //DUMP FUNCTION
    void dump(char sep = '\n');

    int elements;


private:
    SkipListNode* m_head;
    unsigned int m_maxHeight;
};
