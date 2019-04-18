

//#ifndef __CONFIGURE_H_
//#define __CONFIGURE_H_


//typedef unsigned int u_int;
//typedef unsigned char u_char;
//typedef long util_long;

//#endif /* end __CONFIGURE_H_ */



#ifndef HASHFUNC_H
#define HASHFUNC_H
#include <string>
#include "md5.h"

class HashFunc
{
public:
    virtual unsigned int GetKey(std::string str) = 0;
};
#endif
