
#pragma once


#include <unordered_map>
#include <string>
#include <list>

using namespace std;


#define MAXSIZE     (1<<20) //1048576



template <typename Type>
class LRUCache
{
public:
    LRUCache(const unsigned capacity);
    ~LRUCache();
    int  get(const std::string key, Type &value);
    void put(const std::string key, const Type &value);

private:
    struct CacheNode
    {
        std::string key;
        Type value;
        CacheNode(const std::string k, const Type &v) : key(k), value(v) { }
    };

    std::list<CacheNode> cacheList;
    std::unordered_map< std::string, typename std::list<CacheNode>::iterator > cacheMap;
    unsigned int size;
};

template <typename Type>
LRUCache<Type>::LRUCache(const unsigned capacity) {
    if(capacity < MAXSIZE) {
        size = capacity;
    }
}

template <typename Type>
LRUCache<Type>::~LRUCache()
{
    cacheList.clear();
    cacheMap.clear();
    size = 0;
}

template <typename Type>
int LRUCache<Type>::get(const std::string key, Type &value) {
    if (cacheMap.find(key) == cacheMap.end()) {//没有命中
        return -1;
    }

    //命中
    typename std::list<LRUCache::CacheNode>::iterator itfind = cacheMap[key];
    cacheList.erase(itfind);

    //将命中的节点放到链表头部
    cacheList.push_front(CacheNode(key, itfind->value));
    cacheMap[key] = cacheList.begin();

    value = cacheMap[key]->value;
    return 0;
}

template <typename Type>
void LRUCache<Type>::put(const std::string key, const Type &value)
{
    if (cacheMap.find(key) == cacheMap.end()) {// 没有命中, no hit cache
        if (cacheList.size() == size) {
            cacheMap.erase(cacheList.back().key);
            cacheList.pop_back();
        }
        cacheList.push_front(CacheNode(key,value));
        cacheMap[key] = cacheList.begin();
    } else {// 命中
        typename std::list<CacheNode>::iterator it = cacheMap[key];
        cacheList.erase(it);

        cacheList.push_front(CacheNode(key, value));
        cacheMap[key] = cacheList.begin();
    }
}







