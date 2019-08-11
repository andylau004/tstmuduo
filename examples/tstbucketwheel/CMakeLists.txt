

project(tstbucketwheel)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(tstbucketwheel
    tstbucketwheel.h tstbucketwheel.cpp
    main.cpp)


target_link_libraries(tstbucketwheel event muduo_net_cpp11 muduo_base)







