

project(tstbucketwheel)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    tstbucketwheel.h tstbucketwheel.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net_cpp11 muduo_base)







