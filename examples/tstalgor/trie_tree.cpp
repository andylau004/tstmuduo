

#include "trie_tree.h"



#include "boost_use_1.h"


#include <stdio.h>


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include "muduo/base/Timestamp.h"


using namespace std;
using namespace muduo;


#define ALPHABET_SIZE 26


typedef struct _tag_trie_node {
    int count;// 记录该节点代表的单词的个数
    _tag_trie_node* children[ALPHABET_SIZE];// 各个子节点
}trie_node;


trie_node* create_trie_node() {
    trie_node* pNode = new trie_node();
    pNode->count = 0;
    for(int i=0; i < ALPHABET_SIZE; ++i)
        pNode->children[i] = NULL;
    return pNode;
}
void trie_insert(trie_node* root, char* key) {
    trie_node* node = root;
    char* p = key;
    while (*p) {
        if (node->children[*p - 'a'] == NULL) {
            node->children[*p - 'a'] = create_trie_node();
        }
        node = node->children[*p - 'a'];
        ++p;
    }
    node->count += 1;
}
/**
 * 查询：不存在返回0，存在返回出现的次数
*/
int trie_search(trie_node* root, char* key) {
    trie_node* node = root;
    char* p = key;
    while (*p && node) {
        node = node->children[ *p - 'a' ];
        ++p;
    }
    if (NULL == node)
        return 0;
    else {
        return node->count;
    }
}

void trie_tree_1() {
    // 关键字集合
    char keys[][8] = {"the", "a", "there", "answer", "any", "by", "bye", "their"};
    trie_node* root = create_trie_node();

    // 创建trie树
    for(int i = 0; i < 8; i++)
        trie_insert(root, keys[i]);

    // 检索字符串
    char s[][32] = {"Present in trie", "Not present in trie"};
    printf("%s --- %s\n", "the", trie_search(root, "the")>0?s[0]:s[1]);
    printf("%s --- %s\n", "these", trie_search(root, "these")>0?s[0]:s[1]);
    printf("%s --- %s\n", "their", trie_search(root, "their")>0?s[0]:s[1]);
    printf("%s --- %s\n", "thaw", trie_search(root, "thaw")>0?s[0]:s[1]);

}


void yinyongBind() {
    class A
    {
    public:
        virtual void print()
        {
            cout<<"A"<<endl;
        }
    };

    class B:public A
    {
    public:
        /*virtual*/ void print()
        {
            cout<<"B"<<endl;
        }
    };
//    B b;

//    A* pBObj = new B();
//    A& a=b;

//    a.print();
//    pBObj->print();

    A a;
    B b;
    a = b;
a.print();
b.print();
}


#include <sys/time.h>
#include <time.h>
#include <unistd.h>


uint64_t GetCurMilliSecond() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
//    printf("second:%ld\n",tv.tv_sec);  //秒
//    printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

void print_time ()
{
    uint64_t ucur = GetCurMilliSecond();
    std::cout << "ucr=" << ucur << std::endl;
    {
        struct timeval tv;
        struct tm* ptm;
        char time_string[40];
        long milliseconds = 0;

        /* 获得日期时间，并转化为 struct tm。 */
//        gettimeofday(&tv, NULL);
        tv.tv_sec = ucur/1000;
        ptm = localtime (&tv.tv_sec);
        /* 格式化日期和时间，精确到秒为单位。*/

        strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
        /* 从微秒计算毫秒。*/
//        milliseconds = tv.tv_usec / 1000;
//        /* 以秒为单位打印格式化后的时间日期，小数点后为毫秒。*/
        printf ("%s.%03ld\n", time_string, milliseconds);

    }
    sleep(3);

    {
        struct timeval tv;
        struct tm* ptm;
        char time_string[40];
        long milliseconds;

        /* 获得日期时间，并转化为 struct tm。 */
        gettimeofday(&tv, NULL);
        ptm = localtime (&tv.tv_sec);
        /* 格式化日期和时间，精确到秒为单位。*/
        strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
        /* 从微秒计算毫秒。*/
        milliseconds = tv.tv_usec / 1000;
        /* 以秒为单位打印格式化后的时间日期，小数点后为毫秒。*/
        printf ("%s.%03ld\n", time_string, milliseconds);

    }
}



