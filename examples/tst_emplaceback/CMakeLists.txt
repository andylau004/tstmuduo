



project(tst_emplaceback)


set(EXECUTABLE_OUTPUT_PATH ../../)


add_executable(${PROJECT_NAME}
    tst_emplaceback.h tst_emplaceback.cpp
    ${O_GENCPP}
    main.cpp)

#add_executable(${PROJECT_NAME} ${CODE_LIST})

#target_link_libraries(${PROJECT_NAME} muduo_base muduo_net rt thriftnb thrift event ssl crypto)
target_link_libraries(${PROJECT_NAME} pthread boost_system boost_thread muduo_base muduo_net)

#asan




