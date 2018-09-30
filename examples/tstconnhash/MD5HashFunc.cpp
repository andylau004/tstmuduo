

#include "MD5HashFunc.h"

#include "muduo/base/murmurhash.h"



unsigned int MD5HashFunc::GetKey(std::string str){
    const unsigned char* digest = MD5(str).getDigest();//16位 16字节
    unsigned int hash = 0;
    /*
        每四个字节构成一个32位整数
        将四个32位整数相加得到hash值
    */
    for (int i = 0; i < 4; i++)
    {
        hash += ((unsigned int)(digest[i * 4 + 3] & 0xFF) << 24)
            | ((unsigned int)(digest[i * 4 + 2] & 0xFF) << 16)
            | ((unsigned int)(digest[i * 4 + 1] & 0xFF) << 8)
            | ((unsigned int)(digest[i * 4 + 0] & 0xFF));
    }
    return hash;
}


unsigned int MurmurHah32Func::GetKey(std::string str){
    uint32_t hash = base::MurmurHash2(str.data(), str.size(), 82341);
//    std::cout << "hash32=" << hash << std::endl;
    return hash;
}




