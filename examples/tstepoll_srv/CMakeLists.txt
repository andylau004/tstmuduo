


project(tstepoll_srv)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(tstepoll_srv
    tstepoll_srv.h tstepoll_srv.cpp
    main.cpp)


target_link_libraries(tstepoll_srv event  muduo_net muduo_base)







