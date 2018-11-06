


project(tstepoll_cli)


set(EXECUTABLE_OUTPUT_PATH ../../)


add_executable(tstepoll_cli
    tstepoll_cli.h tstepoll_cli.cpp
    main.cpp)


target_link_libraries(tstepoll_cli muduo_net muduo_base)







