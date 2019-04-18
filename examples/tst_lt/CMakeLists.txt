

project(tst-lt)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    tst-lt.h tst-lt.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net muduo_base)







