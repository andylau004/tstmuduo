#ifndef __CREATE_ID_H__2015_12_22__
#define __CREATE_ID_H__2015_12_22__

//#include "tdefine.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define  NEED_MORE_BUFF_LENGTH    (-2048)


//--------------------------------------------------------------------------------------------//
//                                  export function begin


// --------------------循环方式调用，分块调用--------------------
void* NewHshCtx( );
void* Hsh_update( void* pHshCtx, unsigned char* msg, int msglen );
// outHsh 缓冲区长度至少64字节  返回值做了base64编码
int   Hsh_final( void* pHshCtx, unsigned char* outHsh );
// ------------------------------------------------------------

// --------------------简单调用--------------------------------
// outHsh 缓冲区长度至少64字节  返回值做了base64编码
int   Hsh_Simple( unsigned char* msg, int msglen, unsigned char* outHsh );

/*
    * Parameter(s)  : msg      输入明文
    *                 msgLen   输入明文长度 至少64字节 推荐128字节
    *                 outHsh   输出hash缓冲区指针
    * Return        : outHash 成功;
                      NULL    失败;
    *
    * Description   : sha256 Hash 不做base64编码
*/
char*   Hsh_Simple_nob64( char* pUserData, const int DataLen, char* outHsh );
// ------------------------------------------------------------


/**
    * \brief   密钥加密算法
    *
    * \param P  输入的密码
    * \param S  加入的盐 , 32字节
    * \param poutbuffer  最终的密钥 缓冲区至少256字节
    * Return        : 0   成功;
                      < 0 失败;
*/
int Pbkdf2_simple( const char* in_pwd, const char* in_salt, char* poutbuffer );



// new add 2019-3-13 beg

/*
    * Parameter(s)  : pub_key   返回公钥缓冲区
    *                 sizepub   返回公钥缓冲区长度
    *                 priv_key  返回私钥缓冲区
    *                 sizepriv  返回私钥缓冲区长度
    * Return        : 0    成功;
    *                 -1   失败 输入参数不合法;
                      -2   失败 公钥缓冲区长度不够;
    *                 -3   失败 私钥缓冲区长度不够;
    * Description   : 接口内部生成一对 公私钥
*/
int GenEcdsaKeyPair( char* pub_key, int sizepub, char* priv_key, int sizepriv);


/*
    * Parameter(s)  : pUserData   用户数据
    *                 DataLen     数据长度
    *                 privKey     私钥字符串
    *                 pOut        返回签名缓冲区 推荐缓冲区长度256字节
    *                 OutLen      缓冲区长度
    * Return        : 0    成功;
                      -1   失败 输入参数不合法;
    *                 -2   签名失败，请检查输入参数;
    *                 -2048   输出缓冲区长度不够;
    * Description   : 使用 GenEcdsaKeyPair 创建的 私钥 给数据签名
*/
int EcdsaSignByPriKey( char* pUserData, const unsigned int DataLen, const char* privKey,
                       char* pOutSignature, const int OutLen );

/*
    * Parameter(s)  : pubKey      公钥
    *                 inSig       签名后数据，已被base64编码
    *                 pUserData   源数据
    *                 dataLen     数据长度
    * Return        : 0    成功;
                      -1   失败 输入参数不合法;
    * Description   : 使用 GenEcdsaKeyPair 创建的公钥 验证签名
*/
int EcdsaVerifyByPubKey( const char* szPubKey, const char* inSig,
                         char* pUserData, const int dataLen );

/*
    * Parameter(s)  : szPubKey    公钥
    *                 pUserData   用户数据
    *                 dataLen     数据长度
    *                 outbuffer   返回密文缓冲区 推荐缓冲区长度4096字节
    *                 outlen      缓冲区长度
    * Return        : > 0   成功 加密字节数;
                      -1    失败 输入参数不合法;
                      < 0   失败;
    * Description   : 使用 GenEcdsaKeyPair 创建的公钥 加密秘钥
*/
int EccEncryptByPubKey( char* szPubKey, char* pUserData, const int dataLen, char* outbuffer, int outlen );

/*
    * Parameter(s)  : priKey      私钥
    *                 pUserData   密文数据
    *                 lenEncrypted密文长度
    *                 outbuffer   返回明文缓冲区 推荐缓冲区长度4096字节
    *                 outlen      缓冲区长度
    * Return        : > 0   成功 解密字节数;
                      -1    失败 输入参数不合法;
                      < 0   失败;
    * Description   : 使用 GenEcdsaKeyPair 创建的私钥 解密密文
*/
int EccDecryptByPriKey( char* priKey, char* pEncrypted, const int lenEncrypted, char* outbuffer, int outlen );

// new add 2019-3-13 end













/*
    * Parameter(s)  : lpszFileName          输入待签名文件
    *                 lpszPrivKeyFileName   输入私钥文件
    *                 pOut     输出缓冲区
    *                 iOutLen  输出缓冲区长度
    * Return        : 0    成功;
                      -1   失败;
    *                 -2   输出缓冲区长度不够，如果*pOutLen长度不够，接口内赋值适合长度给该指针
                      -4   输入文件不存在;
    * Description   : 使用私钥签名指定文件
*/
int SignFileByPrivKey( const char* lpszFileName, const char* lpszPrivKeyFileName,
                       unsigned char* pOut, int* pOutLen );

/*
    * Parameter(s)  : pInData          输入待签名内存数据
    *                 lpszPrivKeyFileName   输入私钥文件
    *                 pOut     输出缓冲区
    *                 iOutLen  输出缓冲区长度
    * Return        : 0    成功;
                      -1   失败;
    *                 -2   输出缓冲区长度不够，如果*pOutLen长度不够，接口内赋值适合长度给该指针
    * Description   : 重载 使用私钥签名指定文件
*/
int SignBuffByPrivKey( unsigned char* pInBuff, unsigned int InBuffLen, const char* lpszPrivKeyFileName,
                       unsigned char* pOut, int* pOutLen );

/*
    * Parameter(s)  :
    *                 lpszCrtFileName   证书文件
    *                 pInBuff           签名数据
    *                 InBuffLen         签名数据长度
    *                 lpszInfile        源数据文件
    * Return        : 0    成功;
                      -1   失败;
    *                 -2
    * Description   : 输入签名后数据pInBuff 使用证书验证该签名
*/
int VerifySignByCrt( const char* lpszCrtFileName, unsigned char* pInBuff, int InBuffLen,
                     const char* lpszInfile );


/*
    * Parameter(s)  :
    *                 lpszCrtFileName   输入证书文件
    *                 pInSignBuff       签名数据
    *                 InSignLen         签名数据长度
    *                 pSrcBuff          源内存数据
    *                 iSrcLen           源数据长度
    * Return        : 0    成功;
                      -1   失败;
    *                 -2
    * Description   : 输入签名后数据pInSignBuff 使用证书验证该签名
*/
int VerifySign_BuffByCrt( const char* lpszCrtFileName, unsigned char* pInSignBuff, int InSignLen,
                          unsigned char* pSrcBuff, int iSrcLen );




//typedef enum  { Encrypt, Decrypt } Direction;
//typedef enum  { ECB , CBC , CFB1 } WorkMode ;



/*
    * Parameter(s)  : msg      输入明文
    *                 msgLen   输入明文长度
    *                 encMsg   输出密文缓冲区 返回加密数据已做base64解码
    *                 encMsgLen输出密文缓冲区长度
    *                 lpszCrtFile   证书文件
    * Return        : >0 加密后密文长度 成功;
                      -1 失败;
                      -2 如果输出缓冲区长度不够或者encMsg=NULL，接口内把密文需要长度赋值给*encMsgLen
    *
    * Description   : ECDSA证书加密
*/
int EC_EncryptByCrt( const unsigned char* msg, const unsigned int msgLen,
                     unsigned char* encMsg, unsigned int* encMsgLen, const char* lpszCrtFile );


/*
    * Parameter(s)  : encMsg   输入密文缓冲区
    *                 encMsgLen输入密文缓冲区长度
    *                 decMsg   输出明文缓冲区
    *                 decMsgLen输出明文缓冲区长度
    *                 keyfile  私钥文件
    * Return        : >0 解密后明文长度 成功;
                      -1 失败;
                      -2 如果输出缓冲区长度不够或者decMsg=NULL，接口内把密文需要长度赋值给*encMsgLen
    *
    * Description   : ECDSA私钥解密
*/
int EC_DecryptByPri( unsigned char* encMsg, unsigned int encMsgLen,
                     unsigned char* decMsg, unsigned int* decMsgLen, const char* keyfile );


/*
    * Parameter(s)  : pIn      输入缓冲区
    *                 iInLen   输入缓冲区长度
    *                 pOut     输出缓冲区 返回加密数据已做base64解码
    *                 iOutLen  输出缓冲区长度 至少=(iInLen + 16 + 2) / 3 * 4
    *                 encrypt_key  加密KEY，16字节
    *                 iKeyLen      16字节
    *                 workmode 默认CBC模式，其他模式暂不支持
    * Return        : >0 解密后明文数据长度 成功;
                      < 0 失败;
    *
    * Description   : aes_cbc加密
*/
int Cbc_Encrypt( unsigned char* pIn,  const unsigned int iInLen,
                 unsigned char* pOut, int iOutLen,
                 unsigned char* encrypt_key, int iKeyLen );

/*
    * Parameter(s)  : pIn      输入明文
    *                 iInLen   明文长度
    *                 pOut     返回密文缓冲区，缓冲区长度至少跟明文缓冲区长度一样
    *                 iOutLen  缓冲区长度至少跟明文缓冲区长度一样, iOutLen >= iInLen
    *                 encrypt_key  加密KEY，16字节
    *                 iKeyLen      16字节
    * Return        : >0  成功 加密密文长度;
                      < 0 失败;
    *
    * Description   : aes cfb加密
*/
int Cfb_Encrypt_nob64( unsigned char* pIn,  const unsigned int iInLen,
                       unsigned char* pOut, int iOutLen,
                       unsigned char* encrypt_key, int iKeyLen );


/*
    * Parameter(s)  : pIn      输入缓冲区
    *                 iInLen   输入缓冲区长度
    *                 pOut     输出缓冲区
    *                 iOutLen  输出缓冲区长度 至少==iInLen
    *                 decrypt_key  解密KEY，16字节
    *                 iKeyLen      16字节
    *                 workmode 默认CBC模式，其他模式暂不支持
    * Return        : >0 解密后明文数据长度 成功;
                      < 0 失败;
    *
    * Description   : aes_cbc解密
*/
int Cbc_Decrypt( unsigned char* pIn,  int iInLen,
                 unsigned char* pOut, int iOutLen,
                 unsigned char* decrypt_key, int iKeyLen );


/*
    * Parameter(s)  : pIn      输入密文
    *                 iInLen   密文长度
    *                 pOut     返回解密缓冲区，解密缓冲区长度至少跟密文缓冲区长度一样
    *                 iOutLen  返回解密缓冲区长度至少跟 输入密文 长度一样, iOutLen >= iInLen
    *                 encrypt_key  加密KEY，16字节
    *                 iKeyLen      16字节
    * Return        : >0  成功 解密长度;
                      < 0 失败;
    *
    * Description   : aes cfb解密
*/
int Cfb_Decrypt_nob64( unsigned char* pIn,  int iInLen,
                       unsigned char* pOut, int iOutLen,
                       unsigned char* decrypt_key, int iKeyLen );




/*
    * Parameter(s)  : lpszELog      企业elogo
    *                 lpszSrcFile   源文件
    *                 lpszOutFile   密文文件
    * Return        : == 0 成功;
                      <  0 失败;
                      -1   文件信息提取错误，无法备份加密
                      -1024签名校验失败，返回
    *
    * Description   : 加密db文件
*/
int EncDbFile( const char* lpszELog, const char* lpszSrcFile, const char* lpszOutFile );

/*
    * Parameter(s)  : lpszELog      企业elogo
    *                 lpszEncFile   密文文件
    *                 lpszOutFile   源文件
    * Return        : == 0 成功;
                      <  0 失败;
                      -1   文件信息提取错误，无法备份加密
                      -1024签名校验失败，返回
    *
    * Description   : 解密db文件
*/
int DecDbFile( const char* lpszELog, const char* lpszEncFile, const char* lpszOutFile );















#define  EVP_MAX_DG_SIZE     64
typedef struct _tagVRV_ENDATA_HEAD_
{
    unsigned char         dgst[EVP_MAX_DG_SIZE];
    uint64_t              ts;

} VRV_ENDATA_HEAD, *PVRV_ENDATA_HEAD;

#define  SIZE_VRV_ENDATA_HEAD  sizeof(VRV_ENDATA_HEAD)    //72





/**
    * \brief   证书解析
    *
    * \param type   0: 证书文件全路径; 1: 证书文件内容;
    * \param ppoutbuffer  返回json结构
    * Return        : 0   成功;
                      < 0 失败;
*/
int ParseCrt( int inType, const char* crtFile, char** ppoutbuffer );
//int ParseCrtContent( const char* crtFileContent, char** ppoutbuffer );



/**
    * 释放返回内存buffer接口
    * @param poutbuffer
    * @param
    * @return 0 successed; < 0 failed;
*/
void FreeOutBuffer( char* poutbuffer );





/*
 * EncodeData encrypt input data
 * CompanyTag Company Tag Info
 * dataIn     input to Encrypt data
 * inlen      intpu data len
 * dataout	  encrypted data out buffer
 * outlen     outlen >= ( iInLen + 128 + 2 ) / 3 * 4 + 1
 * return     strlen(encypted data)
              if return > 0, successed
              if return < 0, must been failed
*/
int EC60789803D1(const char *CompanyTag, char *dataIn, int inlen, unsigned char *dataout, int outlen);
typedef int (* PFN_EC60789803D1)(const char *CompanyTag, char *dataIn, int inlen, unsigned char *dataout, int outlen);

/*
 * DecodeData decrypt intput encrypted data by EC60789803D1
 * CompanyTag Company Tag Info
 * dataIn     input to Encrypted data by EC60789803D1
 * inlen      input len
 * dataout	  decrypted data out buffer
 * outlen     outlen >= (inlen + 128)
 * return     decode data len
              if return > 0, successed
              if return < 0, must been failed
*/
int EC60789803D2(const char *CompanyTag, char *dataIn, int inlen, unsigned char *dataout, int outlen);
typedef int (*PFN_EC60789803D2)(const char *CompanyTag, char *dataIn, int inlen, unsigned char *dataout, int outlen);


//                                  export function end
//--------------------------------------------------------------------------------------------//





int  b64_encode_Ptr(void* pIn, int iInLen, void* pOut, int iOutLen);
int  b64_decode_Ptr(void* pIn, int iInLen, void* pOut, int iOutLen);


void tst_base64_fun( );
void tst_b64_no_openssl();







void tst_hash_func();







//int tst_export_pubkey_fun( const char* lpszCrtFileName );

//void printf_buffer(const char* title, unsigned char* buffer, int size);
//void printf_buffer_char(const char* title, char* buffer, int size);










//extern "C"
void TestFunction() ;



#ifdef __cplusplus
}
#endif

#endif // __CREATE_ID_H__2015_12_22__
