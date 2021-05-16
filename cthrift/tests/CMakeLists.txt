IF (ENABLE_COVERAGE)
MESSAGE(STATUS "ENABLE_COVERAGE ------------------------------")
    SET(CMAKE_CXX_FLAGS "-g -Wall -O0 -fPIC -DMUDUO_STD_STRING -DHAVE_NETINET_IN_H -DGTEST")
ELSE ()
MESSAGE(STATUS "not ENABLE_COVERAGE ------------------------------")
    SET(CMAKE_CXX_FLAGS "-g -Wall -O2 -fPIC -DMUDUO_STD_STRING -DHAVE_NETINET_IN_H -DGTEST -std=c++11 ")
ENDIF ()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cthrift/cmake")
MESSAGE(STATUS "CMAKE_MODULE_PATH is: ${CMAKE_MODULE_PATH}")
MESSAGE(STATUS "CMAKE_SOURCE_DIR is: ${CMAKE_SOURCE_DIR}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH is: ${EXECUTABLE_OUTPUT_PATH}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -fprofile-arcs")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -fprofile-arcs -ftest-coverage-lgcov")

IF (ENABLE_COVERAGE)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
ENDIF()
MESSAGE(STATUS "CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}")

option(CTHRIFT_DOWNLOAD_GTEST "Download and build a fresh copy of googletest. Requires Internet access." ON)
set(CTHRIFT_SYSTEM_GTEST_SOURCE_DIR "" CACHE PATH "System googletest source directory.")
MESSAGE(STATUS "CTHRIFT_SYSTEM_GTEST_SOURCE_DIR is: ${CTHRIFT_SYSTEM_GTEST_SOURCE_DIR}")
MESSAGE(STATUS "PROJECT_BINARY_DIR is: ${PROJECT_BINARY_DIR}")

include(module)

#if (CTHRIFT_DOWNLOAD_GTEST)
#    include(SetupGtest)
#elseif (CTHRIFT_SYSTEM_GTEST_SOURCE_DIR)
    # add_subdirectory("${CTHRIFT_SYSTEM_GTEST_SOURCE_DIR}" "${PROJECT_BINARY_DIR}/system-googletest-build")
    set(CTHRIFT_SYSTEM_GTEST_SOURCE_DIR "/host_WorkDir/mygitwork/tstmuduo/cthrift/googletest-release-1.6.0/build")
    MESSAGE(STATUS "CTHRIFT_SYSTEM_GTEST_SOURCE_DIR is: ${CTHRIFT_SYSTEM_GTEST_SOURCE_DIR}")
    #else ()
#    message(FATAL_ERROR "Googletest is not available")
#endif ()


INCLUDE_DIRECTORIES(SYSTEM echo)
INCLUDE_DIRECTORIES(SYSTEM ${INCLUDE_OUTPUT_PATH})

SET(ECHO_THRIFT_SRCS
        echo/echo_constants.cpp
        echo/echo_types.cpp
        echo/Echo.cpp
        )

INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${INCLUDE_OUTPUT_PATH})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/cthrift/googletest-release-1.6.0/include/)
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/cthrift/mns_sdk/)
# INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/googletest-src/googletest/include/)
# link_directories(/host_WorkDir/mygitwork/tstmuduo/build/)
# link_directories(/host_WorkDir/mygitwork/tstmuduo/build/muduo/base/)
link_directories(${LIBRARY_OUTPUT_PATH})

# target_link_libraries(tstpoll_cli muduo_net_cpp11 muduo_base_cpp11 rt boost_thread boost_system boost_filesystem)

set(unit_echo ${ECHO_THRIFT_SRCS})

add_library(util_unit_test_main STATIC ${unit_echo} main.cc)
add_library(util_unit_test_main_async STATIC ${unit_echo})


set(TEST_FILE_LIST
#        "cthrift_client_unittest.cc"
        "cthrift_transport_unittest.cc"
        "cthrift_common_unittest.cc"
        "cthrift_sg_agent_unittest.cc"
        "cthrift_util_zk_tools_unittest.cc"
        "cthrift_channel_unittest.cc"
        )

foreach (TEST_FILE_NAME ${TEST_FILE_LIST})

    link_directories(/usr/local/lib/)
    link_directories(/host_WorkDir/mygitwork/tstmuduo/cthrift/googletest-release-1.6.0/build/)
    link_directories(/host_WorkDir/mygitwork/tstmuduo/build/)
    link_directories(/host_WorkDir/mygitwork/tstmuduo/build/muduo/base/)
    link_directories(/host_WorkDir/mygitwork/tstmuduo/build/muduo/net/)

    get_filename_component(test_name ${TEST_FILE_NAME} NAME_WE)
    add_executable(${test_name} ${TEST_FILE_NAME})

    target_link_libraries(${test_name} util_unit_test_main
        -Xlinker "-("libboost_system.a libcthrift.a libmns_sdk.a libmuduo_base_cpp11.a libmuduo_net_cpp11.a ${MUDUO_HTTP_LIBRARY}
        ${ZOOKEEPER_LIBRARY} liboctoidl.a  pthread rt libz.so liblog4cplus.so
        libthrift.a  libthriftnb.a gtest -Xlinker "-)"
        )
    cthrift_add_test(${test_name} ${test_name} CONFIGURATIONS Debug)
endforeach ()

INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/cthrift/mns_sdk/)

MESSAGE(STATUS "--- CMAKE_SOURCE_DIR is: ${CMAKE_SOURCE_DIR}")
MESSAGE(STATUS "--- PROJECT_BINARY_DIR is: ${PROJECT_BINARY_DIR}")


link_directories(/usr/local/lib/)
link_directories(/host_WorkDir/mygitwork/tstmuduo/cthrift/googletest-release-1.6.0/build/)
link_directories(/host_WorkDir/mygitwork/tstmuduo/build/)
link_directories(/host_WorkDir/mygitwork/tstmuduo/build/muduo/base/)
link_directories(/host_WorkDir/mygitwork/tstmuduo/build/muduo/net/)

add_executable(main_async_unittest main_async.cc cthrift_client_worker_unittest.cc)
target_link_libraries(main_async_unittest util_unit_test_main_async
        -Xlinker "-("libz.so libboost_system.a libcthrift.a libmns_sdk.a libmuduo_base_cpp11.a libmuduo_net_cpp11.a ${MUDUO_HTTP_LIBRARY}
        ${ZOOKEEPER_LIBRARY} liboctoidl.a  pthread rt  liblog4cplus.so 
        libthrift.a  libthriftnb.a   
        gtest -Xlinker "-)"
        )
cthrift_add_test(main_async_unittest main_async_unittest CONFIGURATIONS Debug)

#add_custom_target(copyTarget1
#    COMMAND cp -n ${PROJECT_BINARY_DIR}/cthrift/tests/main_async_unittest ${PROJECT_BINARY_DIR}/
#)
#add_dependencies(main_async_unittest copyTarget1)



add_executable(main_svr_unittest main_svr.cc cthrift_server_unittest.cc)
target_link_libraries(main_svr_unittest util_unit_test_main_async
         -Xlinker "-("libz.so libboost_system.a libcthrift.a libmns_sdk.a libmuduo_base_cpp11.a libmuduo_net_cpp11.a ${MUDUO_HTTP_LIBRARY}
         ${ZOOKEEPER_LIBRARY} liboctoidl.a  pthread rt  liblog4cplus.so
         libthrift.a  libthriftnb.a
         gtest -Xlinker "-)"
         )
cthrift_add_test(main_svr_unittest main_svr_unittest CONFIGURATIONS Debug)


add_executable(main_all_unittest main_all.cc ${TEST_FILE_LIST} cthrift_client_worker_unittest.cc)
target_link_libraries(main_all_unittest util_unit_test_main_async
         -Xlinker "-("libz.so libboost_system.a libcthrift.a libmns_sdk.a libmuduo_base_cpp11.a libmuduo_net_cpp11.a ${MUDUO_HTTP_LIBRARY}
         ${ZOOKEEPER_LIBRARY} liboctoidl.a  pthread rt  liblog4cplus.so
         libthrift.a  libthriftnb.a
         gtest -Xlinker "-)"
         )
cthrift_add_test(main_all_unittest main_all_unittest CONFIGURATIONS Debug)









