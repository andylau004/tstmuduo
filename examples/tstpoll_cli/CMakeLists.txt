


project(tstpoll_cli)


set(EXECUTABLE_OUTPUT_PATH ../../)


add_executable(
    tstpoll_cli
    tstpoll_cli.h tstpoll_cli.cpp main.cpp)


target_link_libraries(tstpoll_cli muduo_net_cpp11 muduo_base_cpp11 rt boost_thread boost_system boost_filesystem)

#target_link_libraries(tstpoll_cli muduo_base muduo_net rt boost_thread boost_system boost_filesystem)






