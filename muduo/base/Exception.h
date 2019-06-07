// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_EXCEPTION_H
#define MUDUO_BASE_EXCEPTION_H

#include <muduo/base/Types.h>


#include <string>
#include <sstream>
#include <exception>


namespace muduo
{

class Exception : public std::exception
{
 public:
  explicit Exception(const char* what);
  explicit Exception(const string& what);
  virtual ~Exception() throw();
  virtual const char* what() const throw();
  const char* stackTrace() const throw();

 private:
  void fillStackTrace();

  string message_;
  string stack_;
};




}


class ErasureException : public std::exception
{
public:
    ErasureException(int errcode, const std::string& verbose = "") throw()
    :std::exception(), mErrorCode(errcode), mVerbose(verbose)
    {
        std::ostringstream oss;
        oss << "Error Code: " << mErrorCode << " " << mVerbose;
        mWhat = oss.str();
    }

    virtual ~ErasureException() throw() {}

public: // From std::exception
    virtual const char* what() const throw() { return mWhat.c_str(); }

public:
    int get_error_code() const throw() { return mErrorCode; }

    const std::string& get_verbose() const throw() { return mVerbose; }

protected:
   int mErrorCode;
   std::string mWhat;
   std::string mVerbose;
};

#endif  // MUDUO_BASE_EXCEPTION_H
