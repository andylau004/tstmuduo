

#include "hashfun.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <map>
#include <list>
#include <queue>


#include <string.h>

#include "muduo/base/murmurhash.h"


using namespace std;



void tst_murmurhash_1() {
    char str[120];
    sprintf(str, "%d+p8fjde4-!", 123);
    uint64_t hash = base::MurmurHash64B(str, strlen(str), 82341);
    std::cout << "hash64=" << hash << std::endl;

    {
        char str[120];
        sprintf(str, "%d+p8fjde4-!", 123);
        uint32_t hash = base::MurmurHash2(str, strlen(str), 82341);
        std::cout << "hash32=" << hash << std::endl;
    }
}

// 自己实现hashmap表，实现O(1)查询和写入
// 重复的key使用，list存储
class HashMap {

    size_t m_size = 10000;
    std::vector < std::list <pair < int, int > > > m_hashMap;
public:
    HashMap() {
        m_hashMap.resize( m_size );
    }

    int HashCode(int key) {
        return key % m_size;
    }

    void put(int key, int val) {
        // 根据哈希函数找到对应的链表
        auto& listData = m_hashMap[ HashCode(key) ];

        for (auto &it : listData) {
            // 如果已经存在，根据 key 来更新对应的值
            if (it.first == key) {
                it.second = val;
                return;
            }
        }

        // 插入链表的尾部
        listData.emplace_back(key, val);
    }

    // 根据 key 来获取值
    int get(int key) {
        const auto &listdata = m_hashMap[ HashCode(key) ];
        if (listdata.empty()) {
            return -1;
        }

        for (auto &val : listdata) {
            if (val.first == key) {
                return val.second;
            }
        }
        return -1;
    }

    // 根据 key 删除对应的值
    void remove(int key) {

        auto &listdata = m_hashMap[ HashCode(key) ];
        if (listdata.empty()) {
            return;
        }

        for ( std::list < std::pair<int, int> > ::iterator itfind = listdata.begin();
              itfind != listdata.end(); itfind ++) {
            if (itfind->first == key) {
                listdata.erase(itfind);
                std::cout << "list data erase key=" << key << std::endl;
                return;
            }
        }

    }

    void show() {


        int idx = 0;
        for ( const auto &itkv : m_hashMap ) {

//            auto &listKv = m_hashMap[idx];
            if (itkv.size()) {
                std::cout << "indx=" << idx + 1;// << " key=" ;
            }
            for ( const auto &itpair : itkv ) {
                std::cout << " key=" << itpair.first << " val=" << itpair.second;
            }
            if (itkv.size())
                std::cout << std::endl;

            idx ++;
//            std::cout << std::endl;
        }

//        auto& listData = m_hashMap[ HashCode(key) ];

//        for (auto &it : listData) {
//            // 如果已经存在，根据 key 来更新对应的值
//            if (it.first == key) {
//                it.second = val;
//                return;
//            }
//        }


    }


};

void tst_my_hashmap_1() {
    size_t m_size = 10000;
    std::vector < std::list <pair < int, int > > > m_hashMap;

    // error! will be crash
//    m_hashMap.reserve( m_size );
    // correct! will be work
    m_hashMap.resize( m_size );

    auto& somelist = m_hashMap[ 10 % m_size ];
    std::cout << "somelist size=" << somelist.size() << std::endl;
}

void tst_hashmap_2() {
    HashMap hshmapobj;
    hshmapobj.put( 111, 567 );
    hshmapobj.put( 555, 567 );
    hshmapobj.put( 111, 999 );
    hshmapobj.remove( 111 );
    hshmapobj.show();
}

void tst_hash_fun_entry() {

    tst_hashmap_2(); return;

    tst_my_hashmap_1(); return;

    tst_murmurhash_1(); return;
}
