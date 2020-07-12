



project(tstlog)

set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    AsyncLoggingQueue.h AsyncLoggingQueue.cpp
    tstlog.h tstlog.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME}
    gtest
    muduo_base_cpp11
    muduo_net_cpp11
    rt boost_system boost_filesystem)






