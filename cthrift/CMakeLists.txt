


set(EXECUTABLE_OUTPUT_PATH ../../../)

file(GLOB_RECURSE CURRENT_HEADERS *.h *.hpp *.cpp *.cc)
#source_group("Include" FILES ${CURRENT_HEADERS})


SET(CTHRIFT_SRC
        cthrift_async_callback.h
        cthrift_async_callback.cc
        cthrift_svr.cc
        cthrift_svr.h
        cthrift_name_service.cc
        cthrift_name_service.h
        cthrift_transport.cc
        cthrift_transport.h
        cthrift_tbinary_protocol.cc
        cthrift_tbinary_protocol.h
        cthrift_client_worker.cc
        cthrift_client_worker.h
        cthrift_client.cc
        cthrift_client.h
        cthrift_client_channel.cc
        cthrift_client_channel.h
        mns_sdk/mns_sdk.h
        mns_sdk/mns_worker.h
        mns_sdk/thrift_client.h
        octoidl/naming_common_types.h
        octoidl/ServiceAgent.h
        )

SET(MNS_ZK_SRC
        util/cthrift_mns_imp.cc
        util/cthrift_mns_imp.h
        util/cthrift_config.cc
        util/cthrift_config.h
        util/cthrift_common.cc
        util/cthrift_common.h
        util/cthrift_ns_imp.cc
        util/cthrift_ns_imp.h
        util/cthrift_zk_client.cc
        util/cthrift_zk_client.h
        util/zk_tools.cc
        util/zk_tools.h
        util/zk_client.cc
        util/zk_client.h
        util/log4cplus.cc
        util/log4cplus.h
        )

    aux_source_directory(cthrift      CTHRIFT_SRC)
    aux_source_directory(cthrift/util CTHRIFT_UTIL)


#    include_directories(/usr/local/include/c++/4.8.5/)
    include_directories(/usr/local/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thrid/include)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thrid/include/thrift)

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses -std=c++11" )

ADD_LIBRARY(cthrift STATIC ${CTHRIFT_SRC} ${MNS_ZK_SRC})
#ADD_LIBRARY(cthrift_dynamic SHARED ${CTHRIFT_SRC} ${MNS_ZK_SRC})
target_link_libraries(cthrift log4cplus muduo_base muduo_net pthread boost_system boost_thread muduo_base muduo_net rt thriftnb thrift event ssl crypto)

#SET_TARGET_PROPERTIES(cthrift_dynamic PROPERTIES OUTPUT_NAME "cthrift")

#EXECUTE_PROCESS(COMMAND find ${CTHRIFT_SRC_PATH}/cthrift  -path ${CTHRIFT_SRC_PATH}/cthrift/util -prune -o  -name *.h -exec cp -t ${INCLUDE_CTHRIFT_OUTPUT_PATH} {} \;)
#EXECUTE_PROCESS(COMMAND find ${CTHRIFT_SRC_PATH}/cthrift  -path ${CTHRIFT_SRC_PATH}/cthrift/util -prune -o  -name *.tcc -exec cp -t ${INCLUDE_CTHRIFT_OUTPUT_PATH} {} \;)

#EXECUTE_PROCESS(COMMAND find ${CTHRIFT_SRC_PATH}/cthrift/util -name *.h -exec cp -t ${INCLUDE_UTIL_OUTPUT_PATH} {} \;)

#if (NOT CMAKE_BUILD_NO_TESTS)
#    ADD_SUBDIRECTORY(tests)
#endif ()
