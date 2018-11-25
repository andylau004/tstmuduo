



project(transferserver)


set(EXECUTABLE_OUTPUT_PATH ../../)

#aux_source_directory(. CODE_LIST)

include_directories(../gen-cpp)
#include_directories(../thrift-file)

aux_source_directory(../gen-cpp O_GENCPP)
#aux_source_directory(../thrift-file O_THRIFT_FILE)

add_executable(${PROJECT_NAME}
    ../thrift-file/transfer.thrift
    transferserver.h transferserver.cpp
    server.hpp server.cpp
    common_stucts.hpp common_stucts.cpp
    iothreadmanager.hpp iothreadmanager.cpp
    iothread.hpp iothread.cpp
    threadex.hpp threadex.cpp
    clientctx.hpp clientctx.cpp
    sigslot.h sigslot.cc
    transport.hpp transport.cpp
    frametransport.hpp frametransport.cpp
    processor.h processor.cpp
    testprocessor.h testprocessor.cpp
    clientctxmgr.h clientctxmgr.cpp

    ex_event_handler.h ex_event_handler.cpp
    ${O_GENCPP} ${O_THRIFT_FILE}
    main.cpp)

#add_executable(${PROJECT_NAME} ${CODE_LIST})

target_link_libraries(${PROJECT_NAME} pthread boost_system boost_thread muduo_base muduo_net rt  event ssl crypto  thrift  thriftnb  thriftz      )

#asan




