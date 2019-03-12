

project(tst_rdshamem)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}
    opt_time.h
    rdfun.h rdfun.cpp
    shm_queue.h shm_queue.cpp
    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net muduo_base)







