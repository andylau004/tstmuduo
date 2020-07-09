

project(cthr_cli)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    echo_types.h echo_types.cpp
    echo_constants.h echo_constants.cpp

    Echo.h Echo.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} muduo_net_cpp11 muduo_base cthrift)







