

#pragma once



#include <iostream>
#include <sstream>


#include <queue>


#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <fstream>








using namespace std;









void TestMemberFunction();

void TestBoostBind();






class SystemHelp {
public:

    SystemHelp(const SystemHelp&);
    SystemHelp(SystemHelp&&);
    SystemHelp& operator=(const SystemHelp&);
    SystemHelp& operator=(SystemHelp&&);
    SystemHelp() : id(0), title(), parentId(0), helpType(0), linkType(0), clientType(), displayStatus(0), linkUrl(), detailUrl(), content(), sortNum(0), createTime(0), updateTime(0), status(0) {
    }

    virtual ~SystemHelp() throw();
    int64_t id;
    std::string title;
    int32_t parentId;
    int32_t helpType;
    int32_t linkType;
    std::string clientType;
    int32_t displayStatus;
    std::string linkUrl;
    std::string detailUrl;
    std::string content;
    int32_t sortNum;
    int64_t createTime;
    int64_t updateTime;
    int32_t status;
};















