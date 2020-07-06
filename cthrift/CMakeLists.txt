



SET(CTHRIFT_SRC
        cthrift_svr.cc
        cthrift_svr.h
        cthrift_name_service.cc
        cthrift_name_service.h
        cthrift_transport.cc
        cthrift_transport.h
        cthrift_client_worker.cc
        cthrift_client_worker.h
        cthrift_client.cc
        cthrift_client.h
        cthrift_client_channel.cc
        cthrift_client_channel.h
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
    include_directories(/usr/local/include/c++/4.8.5/)
    include_directories(/usr/local/include)


ADD_LIBRARY(cthrift STATIC ${CTHRIFT_SRC} ${MNS_ZK_SRC})
#ADD_LIBRARY(cthrift_dynamic SHARED ${CTHRIFT_SRC} ${MNS_ZK_SRC})

#SET_TARGET_PROPERTIES(cthrift_dynamic PROPERTIES OUTPUT_NAME "cthrift")

#EXECUTE_PROCESS(COMMAND find ${CTHRIFT_SRC_PATH}/cthrift  -path ${CTHRIFT_SRC_PATH}/cthrift/util -prune -o  -name *.h -exec cp -t ${INCLUDE_CTHRIFT_OUTPUT_PATH} {} \;)
#EXECUTE_PROCESS(COMMAND find ${CTHRIFT_SRC_PATH}/cthrift  -path ${CTHRIFT_SRC_PATH}/cthrift/util -prune -o  -name *.tcc -exec cp -t ${INCLUDE_CTHRIFT_OUTPUT_PATH} {} \;)

#EXECUTE_PROCESS(COMMAND find ${CTHRIFT_SRC_PATH}/cthrift/util -name *.h -exec cp -t ${INCLUDE_UTIL_OUTPUT_PATH} {} \;)

#if (NOT CMAKE_BUILD_NO_TESTS)
#    ADD_SUBDIRECTORY(tests)
#endif ()
