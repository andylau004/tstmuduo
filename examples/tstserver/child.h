




#pragma once

#include <vector>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>



class CParent;

typedef boost::shared_ptr< CParent > ParentPtr;
typedef boost::weak_ptr< CParent >   wk_ParentPtr;


class CChild {
public:
    CChild() {
        std::cout << "CChild construct..." << std::endl;
    }
    ~CChild() {
        std::cout << "CChild destruct..." << std::endl;
    }

    ParentPtr m_parentPtr;
};



