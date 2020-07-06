



#set(base_SRCS

#    Error.h
#    shared_mem.h

#    timeinterval.h
#    timeinterval.cpp

#    duration.h
#    duration.inl.h
#    duration.cc

#    event_watcher.h
#    event_watcher.cpp

#    libevent.h
#    libevent.cc

#    errcode.h
#    exception.h
#    thrift_connection_pool.h
#    thrift_connection_pool.cpp
#    utils.h
#    utils.cpp

#    thrift_helper.h
#    thrift_helper.cpp

#    color.h
#    color.cpp
#    murmurhash.h
#    murmurhash.cpp

#    BinaryTree.cpp

#    Basic.cpp
##  AsyncLogging.cc
#  Condition.cc
#  CountDownLatch.cc
#  Date.cc
#  Exception.cc
##  FileUtil.cc
##  LogFile.cc
#  Logging.cc
#  LogStream.cc
##  ProcessInfo.cc
#  Timestamp.cc
#  TimeZone.cc
#  Thread.cc
#  ThreadPool.cc
#  convert.cpp
#  common.cpp
#  common.h
#  timerutil.cpp

#    noncopyable.h
#    Singleton.h

#  )
file(GLOB base_SRCS *.cpp *.c *.cc */*.cc *.h */*.h)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses -std=c++11" )
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses" )

add_library(muduo_base ${base_SRCS})
target_link_libraries(muduo_base pthread rt boost_system)
set_target_properties(muduo_base PROPERTIES COMPILE_FLAGS "-std=c++11")

add_library(muduo_base_cpp11 ${base_SRCS})
target_link_libraries(muduo_base_cpp11 pthread rt)
set_target_properties(muduo_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++11")
#set_target_properties(muduo_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

#install(TARGETS muduo_base DESTINATION lib)
#install(TARGETS muduo_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/muduo/base)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#  add_subdirectory(tests)
#endif()
