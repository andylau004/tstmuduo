

project(tst_sslconnmysql)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(${PROJECT_NAME}

    main.cpp)


target_link_libraries(${PROJECT_NAME} event muduo_net muduo_base mysqlclient)







