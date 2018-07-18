

#include "parent.h"



#include "pubhead.h"



CTest_enabled_shared::CTest_enabled_shared() {
    std::cout << "CTest_enabled_shared ctor..." << std::endl;
}
CTest_enabled_shared::~CTest_enabled_shared() {
    std::cout << "CTest_enabled_shared dtor..." << std::endl;
}

void CTest_enabled_shared::printTag() {
    CTest_enabled_shared_Ptr guardThis(shared_from_this());

    std::cout << "printTag...guardThis.use_count=" << guardThis.use_count() << std::endl;
}
