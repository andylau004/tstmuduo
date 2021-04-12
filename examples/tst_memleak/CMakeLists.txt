

project(tstmemleak)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    tst-meml.h tst-meml.cpp
    tstperf.h tstperf.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} muduo_net muduo_base)







