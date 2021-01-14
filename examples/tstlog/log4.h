

#pragma once


#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>


void init_logger(const std::string& conf);

#define PRINT_LOG_THRIFT_STRING(x) \
    {                   \
        std::ostringstream stream;  \
        x.printTo(stream);          \
        LOG4CPLUS_INFO(sLogger, __FUNCTION__ << ",tostring:"   \
                        << stream.str());   \
    }\

extern log4cplus::Logger gLogger;
extern log4cplus::Logger sLogger;
extern log4cplus::Logger fLogger;


#define LOG_INFO(input) \
    do { \
        LOG4CPLUS_INFO(sLogger, __FUNCTION__ << "? " << input); \
    } while (0)

#define LOG_ERROR(input) \
    do { \
        LOG4CPLUS_ERROR(sLogger, __FUNCTION__ << "? " << input); \
    } while (0)

#define LOG_DEBUG(input) \
    do { \
        LOG4CPLUS_DEBUG(sLogger, __FUNCTION__ << "? " << input); \
    } while (0)

#define LOG_WARN(input) \
    do { \
        LOG4CPLUS_WARN(sLogger, __FUNCTION__ << "? " << input); \
    } while (0)

#define LOG_INFO_NO_FUNCTION(input) \
    do { \
        LOG4CPLUS_INFO(sLogger, input); \
    } while (0)


