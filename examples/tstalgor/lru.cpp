


#include "boost_use_1.h"


#include <stdio.h>
#include <iostream>
#include <unordered_map>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include "muduo/base/Timestamp.h"


//using namespace std;
//using namespace muduo;


//// 双向链表的节点结构
//struct LRUCacheNode {
//    int key;
//    int value;
//    LRUCacheNode* prev;
//    LRUCacheNode* next;
//    LRUCacheNode():key(0),value(0),prev(NULL),next(NULL){}
//};


//class LRUCache
//{
//private:
//    unordered_map<int, LRUCacheNode*> m;  // 代替hash_map
//    LRUCacheNode* head;     // 指向双链表的头结点
//    LRUCacheNode* tail;     // 指向双链表的尾结点
//    int capacity;           // Cache的容量
//    int count;              // 计数
//public:
//    LRUCache(int capacity);       // 构造函数
//    ~LRUCache();                  // 析构函数
//    int  get(int key);             // 查询数据项
//    void set(int key, int value); // 未满时插入,已满时替换
//private:
//    void removeLRUNode();                 // 删除尾结点（最久未使用）
//    void detachNode(LRUCacheNode* node);    // 分离当前结点
//    void insertToFront(LRUCacheNode* node); // 节点插入到头部
//};


//LRUCache::LRUCache(int capacity)
//{
//    this->capacity = capacity;
//    this->count = 0;
//    head = new LRUCacheNode;
//    tail = new LRUCacheNode;
//    head->prev = NULL;
//    head->next = tail;
//    tail->prev = head;
//    tail->next = NULL;
//}

//LRUCache::~LRUCache()
//{
//    delete head;
//    delete tail;
//}

//int LRUCache::get(int key)
//{
//    if(m.find(key) == m.end())  // 没找到
//        return -1;
//    else
//    {
//        LRUCacheNode* node = m[key];
//        detachNode(node);      // 命中，移至头部
//        insertToFront(node);
//        return node->value;
//    }
//}

//void LRUCache::set(int key, int value)
//{
//    if(m.find(key) == m.end())  // 没找到
//    {
//        LRUCacheNode* node = new LRUCacheNode;
//        if(count == capacity)   // Cache已满
//            removeLRUNode();

//        node->key = key;
//        node->value = value;
//        m[key] = node;          // 插入哈希表
//        insertToFront(node);    // 插入链表头部
//        ++count;
//    }
//    else
//    {
//        LRUCacheNode* node = m[key];
//        detachNode(node);
//        node->value = value;
//        insertToFront(node);
//    }
//}

//void LRUCache::removeLRUNode()
//{
//    LRUCacheNode* node = tail->prev;
//    detachNode(node);
//    m.erase(node->key);
//    --count;
//}

//void LRUCache::detachNode(LRUCacheNode* node)
//{
//    node->prev->next = node->next;
//    node->next->prev = node->prev;
//}


//void LRUCache::insertToFront(LRUCacheNode* node)
//{
//    node->next = head->next;
//    node->prev = head;
//    head->next = node;
//    node->next->prev = node;
//}
