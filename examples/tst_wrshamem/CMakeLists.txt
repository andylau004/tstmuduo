

project(tst_wrshamem)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    ../tst_rdshamem/opt_time.h
    wrfun.h wrfun.cpp
    ../tst_rdshamem/shm_queue.h ../tst_rdshamem/shm_queue.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net muduo_base)







