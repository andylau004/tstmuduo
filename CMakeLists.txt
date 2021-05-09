cmake_minimum_required(VERSION 2.8)

project(tstfun)

set(CXX_FLAGS
    -g
    # -DVALGRIND
    -DMUDUO_STD_STRING
#    -D__GXX_EXPERIMENTAL_CXX0X__
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wno-unused-parameter
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -march=native
    # -MMD
#     -std=c++0x
    -std=c++11
    -rdynamic
    )

file(GLOB_RECURSE CURRENT_HEADERS *.h *.hpp *.cpp *.cc)
source_group("Include" FILES ${CURRENT_HEADERS})


option( _debug "Debug option" ON )
#add_definitions(-D__GXX_EXPERIMENTAL_CXX0X__)

#include_directories(/usr/local/include/c++/4.8.5/)
include_directories(/usr/local/include)
include_directories(/usr/include/mysql)

include_directories("${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/thirdpart/rapidjson)
include_directories(${CMAKE_SOURCE_DIR}/thirdpart/thrift/thrift-0.9.3)
include_directories(${CMAKE_SOURCE_DIR}/thirdpart/thrift/thrift-0.9.3/lib/cpp/src)
#include_directories(${PROJECT_SOURCE_DIR}/thirdpart/concurrentqueue)
#include_directories(${PROJECT_SOURCE_DIR}/thirdpart/concurrentqueue/concurrentqueue-src)

aux_source_directory(. SRC_LIST)
aux_source_directory(muduo/base BASE_LIST)
aux_source_directory(muduo/net  NET_LIST)
aux_source_directory(muduo/net/poller  POLLER_LIST)
#aux_source_directory(poco              POCO_LIST)

aux_source_directory(cthrift      CTHRIFT_SRC)
aux_source_directory(cthrift/util CTHRIFT_UTIL)

#aux_source_directory(cthrift/mns_sdk MNS_SDK_SRC)
#aux_source_directory(cthrift/mns_sdk/util MNS_SDK_UTIL_SRC)
#aux_source_directory(cthrift/octoidl OCTOIDL_SRC)


link_directories(/usr/lib64/mysql)
link_directories(/usr/local/lib/)
#link_directories(/opt/gmssl/lib/)
link_directories(${LIBRARY_OUTPUT_PATH})


add_subdirectory(thirdpart/thrift)
#add_subdirectory(thirdpart/concurrentqueue)
#add_subdirectory(thirdpart/concurrentqueue-src)
add_subdirectory(muduo/base)
add_subdirectory(muduo/net)

add_subdirectory(cthrift)
add_subdirectory(examples)

#target_link_libraries(${PROJECT_NAME} uuid pthread log4cplus boost_thread boost_system boost_filesystem )
#target_link_libraries(${PROJECT_NAME} boost_system rt mysqlcppconn )

# if(_debug)
#     message("here, build debug version")
#     add_definitions(-D_DEBUG_)
# #-std=c++11
#     set(CMAKE_BUILD_TYPE Debug)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -Wno-deprecated  -std=c++11")
# #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -Wno-deprecated ")
# else()
#     message("here, build release version")
#     set(CMAKE_BUILD_TYPE Release)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O2 -Wno-deprecated -std=c++11")
# endif()





