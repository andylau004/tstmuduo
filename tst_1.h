
#pragma once

#define ARRAY_SIZE(array)  (sizeof (array) / sizeof (array[0]))


#include <list>
#include <string>

using namespace std;


//#define SETVALUE(b, t, i, v) if (b) {stmt->set##t((i),(v));}
//#define SETVALUE_EX(b, t, i, v1, v2) if (b) {stmt->set##t((i),(v1));} else {stmt->set##t((i),(v2));}
//#define SPLICESQL(b, x, y) if (b) { (x) += (y);}
//#define SPLICESQL_EX(b, x, y, z) if (b) { (x) += (y); ++(z);}

//#define DMYSQLPJ(b,c,x,y) if(b) { (x) += (y); c = true;}

//#define REMOVE_LAST_COMMA(x) { size_t pos = x.find_last_of(','); \
//                                if (pos == std::string::npos) return false; (x) = (x).substr(0, pos); }

//inline void remove_last_comma(std::string& str) {
//    size_t pos = str.find_last_of(',');
//    if (pos != std::string::npos) {
//        str = str.substr(0, pos);
//    }
//}



extern int tst_epoll_1();




extern void tst_Tcp_server1();

extern void tst_mt_cow();






