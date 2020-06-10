
project(tstpoll_srv)

set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(${PROJECT_NAME}
    tstpoll_srv.h tstpoll_srv.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME}  event  muduo_net muduo_base)







