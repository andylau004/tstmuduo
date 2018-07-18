


#pragma once



#include <stdio.h>
#include <sstream>
#include <iostream>

#include <vector>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>


#include <boost/enable_shared_from_this.hpp>


class CChild;

typedef boost::shared_ptr< CChild >  CChildPtr;
typedef boost::weak_ptr< CChild >    wk_CChildPtr;

class CParent {
public:
    CParent() {
        std::cout << "CParent construct..." << std::endl;
    }
    ~CParent() {
        std::cout << "CParent destruct..." << std::endl;
    }

    wk_CChildPtr m_childPtr;
//    CChildPtr m_childPtr;
};

//typedef boost::shared_ptr< CParent > ParentPtr;




class CTest_enabled_shared :
        public boost::enable_shared_from_this<CTest_enabled_shared>
{
public:
    CTest_enabled_shared();
    ~CTest_enabled_shared();
    void printTag();
};




