

project(tsteventfd)


set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(${PROJECT_NAME}
    tsteventfd.h tsteventfd.cpp
    getchunkedclient.h getchunkedclient.cpp
    ${O_GENCPP}
    main.cpp)


target_link_libraries( ${PROJECT_NAME} pthread boost_system boost_thread muduo_base muduo_net rt thriftnb thrift event ssl crypto )








