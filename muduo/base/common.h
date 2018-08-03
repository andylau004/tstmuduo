#ifndef ONEPIECE_BASE_COMMON_H_
#define ONEPIECE_BASE_COMMON_H_

#include <stdio.h>
#include <string>
#include <stdint.h>

#include <errno.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <sys/stat.h>

#include <limits.h>
#include <time.h>

#include <algorithm>
#include <sys/time.h>
#include <ctype.h>


#include <map>
#include <vector>





// #include <boost/algorithm/string.hpp>
// #include <boost/filesystem/path.hpp>
// #include <boost/filesystem/operations.hpp>
// #include <boost/format.hpp>

#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string.h>
#include <errno.h>

#include <set>
#include <map>
#include <vector>




//#define LINKDOOD_CFG_PATH 	"/data/linkdood/im/conf/liandoudou.conf"
#define LINKDOOD_CFG_PATH 	"./liandoudou.conf"
#define SERVER_CFG_PATH     "/data/linkdood/im/conf/servconfig.json"


#define VRV_ERR -1
#define VRV_OK 0

#define HYPHEN "-"
#define SEP "_"

typedef int SOCKET;
#define INVALID_SOCKET -1


#define UNUSED(x) ((void)x)

template <class T>
void delete_object(T *& t) {
    if (t) {
        delete t;
        t = NULL;
    }
}

template <class T>
void delete_object_array(T ** t) {
    if (t && *t) {
        delete [] (*t);
        (*t) = NULL;
    }
}

#define IDLE_CTX_TIMEOUT 60   // 1 min
#define IDLE_CTX_REMOVE  5    //remove idle_ctx numbers once

#define ERR_EVENT_BASE -1
#define ERR_EVENT_FALSE -2
#define SUCCESS 0

#define ERR_EAGAIN EAGAIN
#define ERR_EWOULDBLOCK EWOULDBLOCK
#define ERR_EINVAL EINVAL
#define ERR_EINTR EINTR

#define ERR_ECONNRESET ECONNRESET
#define ERR_ENOTCONN ENOTCONN
#define ERR_ETIMEDOUT ETIMEDOUT
#define ERR_EPIPE EPIPE


#define MAX_THREAD_COUNT 8

#define LISTEN_BACKLOG_AP 512

//buddy
#define BODDY_DB_ERROR      100006591

//buddyverify
#define BODDYVF_DB_ERROR    100007091

//chat
#define CHAT_DB_ERROR       100009591

//group
#define GROUP_DB_ERROR      100011091

//groupfile
#define GROUPFILE_DB_ERROR  100012591

//groupmember
#define GROUPMB_DB_ERROR    100011591

//groupverify
#define GROUPVF_DB_ERROR    100012091

//imageverify
#define IMAGE_DB_ERROR      100015591

//login
#define LOGIN_DB_ERROR      100008091
#define LOGIN_DB_ERROR_TICKETBEAN_RESSIGN      99

//messagestorage
#define MESSAGEST_DB_ERROR  100010091

//online
#define ONLINE_DB_ERROR     100008591

//recommend
#define RECOMMEND_DB_ERROR  100006091

//registeruser
#define REG_DB_ERROR        100005591

//setting
#define SETTING_DB_ERROR    100005091

//timestamp
#define TS_DB_ERROR         100004091

//user
#define USER_DB_ERROR       100004591

//verifybox
#define VERIFYBOX_DB_ERROR  100007591

//verifycode
#define VERIFYCODE_DB_ERROR 100015091

enum {
    EntOrgTimeStamp  = 11,
    EntListTimeStamp = 12
};

enum TypeProcess {
    P_UNKNOWN = 0,
    P_APPROCESS,
    P_BGPROCESS,      //background processor
    P_JAVAPROCESS
};


enum TypeProtocol {
    PRO_UNKNOWN = 0,
    PRO_BINARY,
    PRO_JSON
};

const int32_t g_basic_timestamp_type[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 21 };
const int     g_basic_timestamp_size   = sizeof(g_basic_timestamp_type) / sizeof(int32_t);

const int32_t g_basic_ts_type[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 21, 23 };
const int     g_basic_ts_size   = sizeof(g_basic_ts_type) / sizeof(int32_t);


#define SETVALUE(b, t, i, v) if (b) {stmt->set##t((i),(v));}
#define SETVALUE_EX(b, t, i, v1, v2) if (b) {stmt->set##t((i),(v1));} else {stmt->set##t((i),(v2));}
#define SPLICESQL(b, x, y) if (b) { (x) += (y);}
#define SPLICESQL_EX(b, x, y, z) if (b) { (x) += (y); ++(z);}

#define DMYSQLPJ(b,c,x,y) if(b) { (x) += (y); c = true;}

#define REMOVE_LAST_COMMA(x) { size_t pos = x.find_last_of(','); \
                                if (pos == std::string::npos) return false; (x) = (x).substr(0, pos); }

inline void remove_last_comma(std::string& str) {
    size_t pos = str.find_last_of(',');
    if (pos != std::string::npos) {
        str = str.substr(0, pos);
    }
}

//#define UNSUPPORTSERVICECODE 11102


#define URGENT_SERVICE_ID   10001L
#define system_sendID       10000L



#include "thirdpart/rapidjson/document.h"
#include "thirdpart/rapidjson/stringbuffer.h"
#include "thirdpart/rapidjson/writer.h"
#include "thirdpart/rapidjson/document.h"
#include "thirdpart/rapidjson/filereadstream.h"


//#include <log4cplus/loggingmacros.h>
//#include <log4cplus/configurator.h>

//#include <log4cplus/logger.h>
//#include <log4cplus/helpers/loglog.h>
//#include <log4cplus/helpers/stringhelper.h>


#include "timerutil.h"



// convert<int8_t> 获取到ASCII码
template <typename out, class in>
out convert(const in & value) {
    std::stringstream stream;
    stream << value;
    out r;
    stream >> r;
    return r;
}

// convert 获取到整数值
template <class in>
int8_t convert(const in & value) {
    std::stringstream stream;
    stream << value;
    int16_t r;
    stream >> r;
    return (int8_t)r;
}

// 针对int8, 单独写的模板
template <class out>
out int8_to_type(const int8_t& value) {
    int v = (int)value;
    std::stringstream stream;
    stream << v;
    out r;
    stream >> r;
    return r;
}

template <class in>
int8_t type_to_int8(const in& value) {
    std::stringstream stream;
    stream << value;
    int r;
    stream >> r;
    return (int8_t)r;
}

template <class T>
T get_value(const std::string & str) {
    std::istringstream iss(str);

    T value;
    iss >> value;
    return value;
}


bool        stringTrimEx( std::string& sInput, char c = ' ' );
//	转换为大写
void        MakeUpper( std::string& sInput ) ;

//	转换为小写
void        MakeLower( std::string& sInput ) ;



bool CreateFullDir(const char* lpszdir);
bool CreateFullDir(const std::string& indir);


bool isFileExist(const std::string& infile);
bool isFileExist(const char* lpszfile);


bool isPathExist(const char* lpszdir);
bool isPathExist(const std::string& indir);



std::string GetCurrentDir();




bool CopyFileEx( const char* lpszSrc, const char* lpszDst );
bool CopyFileEx( const std::string& srcfile, const std::string& dstfile );


bool save_file( const char* lpfilename, const std::string& sinput );
bool save_file( const char* lpfilename, char* inbuffer, size_t insize );

char* GetSmallFileContent( const char* lpszFileName, unsigned int* pFileSize );
unsigned int GetFileSize( const char* lpszFileName );



bool get_string_value(const rapidjson::Document& d, const char* name, std::string& value);

std::string SplitString( const std::string& s, std::set<std::string>& v, const std::string& c );
std::string SplitString( const std::string& s, std::set<std::string>& v, const char* lpszsep );




void GetStringFromJson( const rapidjson::Document& jsonReader, const char* szName, std::string& outString, const char* lpszDefault );
void GetInt64FromJson( const rapidjson::Document& jsonReader, const char* szName, int64_t& outVal, int64_t inDefault );
void GetIntFromJson( const rapidjson::Document& jsonReader, const char* szName, int32_t& outVal, int32_t inDefault );




uint64_t stringToUint64_t(const char* lpszVal);



//ConfigurationUtil& G_GetConfigObj();


uint64_t GetCurMilliSecond();
std::string GetCurMilliSecond_string();

int64_t  GetRandom64();




///check time function
class checktime{
public:
    checktime(const std::string &name, const int64_t boundary):
        _boundary(boundary), _name(name),_info("") {
#if 0
        gettimeofday(&_starttime, 0);
#endif
        start_ = TimerUtils::get_current_nanotime();
    }
    checktime(const std::string &name, const std::string &info,const int64_t boundary):
        _boundary(boundary), _name(name),_info(info) {
#if 0
        gettimeofday(&_starttime, 0);
#endif
        start_ = TimerUtils::get_current_nanotime();
    }
    ~checktime() {
        check_dump("");
    }

    void check(const std::string &info){
        check_dump(info);
    }

private:
    void check_dump(const std::string &info){
#if 0
        struct timeval end;
        gettimeofday(&end, 0);
        //ms
        int32_t diff = ((end.tv_sec * 1000000 + end.tv_usec) - (_starttime.tv_sec*1000000 + _starttime.tv_usec))/1000 ;
#endif
        int32_t diff = (int32_t)((TimerUtils::get_current_nanotime() - start_) / 1e6);
        dump(_name, _info, info, _boundary, diff);
    }
    void dump(const std::string& name, const std::string& _info, const std::string& info, const int32_t _boundary, const int32_t diff);

    struct timeval _starttime;
    int64_t start_;
    int32_t _boundary; //ms
    std::string _name;
    std::string _info;
};




#define CHECK_TIME() \
    ::checktime _check_time(__FUNCTION__, 200);//default 200ms
#define CHECK_TIME1(self_def) \
    ::checktime self_def(__FUNCTION__, #self_def, 200);//default 200ms





#endif  // ONEPIECE_BASE_COMMON_H_

