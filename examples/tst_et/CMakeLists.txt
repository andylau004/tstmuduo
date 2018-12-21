

project(tst-et)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    tst-et.h tst-et.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net muduo_base)







