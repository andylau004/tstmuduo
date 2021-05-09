



project(tstboost)


set(EXECUTABLE_OUTPUT_PATH ../../)




add_executable(${PROJECT_NAME}
    tstboost.h tstboost.cpp
    main.cpp)

target_link_libraries(${PROJECT_NAME} pthread  boost_timer boost_system boost_thread muduo_base muduo_net rt boost_chrono)






