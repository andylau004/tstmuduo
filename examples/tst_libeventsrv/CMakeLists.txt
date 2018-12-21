

project(tstlesrv)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    tstsrv.h tstsrv.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net muduo_base)







