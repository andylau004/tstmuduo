
#pragma once





#include <pthread.h>
#include <queue>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn/prepared_statement.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>


#include "common.h"



using namespace std;



// create my bst tree
class BstNode {
public:
    int key_;
    BstNode* lchild_;
    BstNode* rchild_;
    BstNode* parent_;
public:
    BstNode(int key, BstNode* lchild, BstNode* rchild, BstNode* parent) :
        key_( key ),
        lchild_(lchild),
        rchild_(rchild),
        parent_(parent)
    {}
    BstNode(int key) :
        key_( key ),
        lchild_(nullptr),
        rchild_(nullptr),
        parent_(nullptr)
    {}
};




extern int test_move1();



extern int test_move2();



extern void tst_shared_ptr_1();










