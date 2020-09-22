



project(tstc11)

set(EXECUTABLE_OUTPUT_PATH ../../)

include_directories(../gen-cpp)

add_executable(${PROJECT_NAME}
    threadsepcific.h  threadsepcific.cpp
    boostany.h boostany.cpp
    c11fun.h c11fun.cpp
    tstbacktrace.h tstbacktrace.cpp
    branch_predict.h branch_predict.cpp
    main.cpp)


#target_link_libraries(${PROJECT_NAME} muduo_base muduo_net rt thriftnb thrift event ssl crypto)
target_link_libraries(${PROJECT_NAME} pthread boost_filesystem boost_system boost_thread muduo_base muduo_net rt thriftnb thrift event ssl crypto   )

#asan




