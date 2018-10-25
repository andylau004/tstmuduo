


project(tstpoll_cli)


set(EXECUTABLE_OUTPUT_PATH ../../)


add_executable(tstpoll_cli
    tstpoll_cli.h tstpoll_cli.cpp
    main.cpp)


target_link_libraries(tstpoll_cli muduo_net muduo_base)







