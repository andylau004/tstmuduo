



project(transferclient)


set(EXECUTABLE_OUTPUT_PATH ../../)

#aux_source_directory(. CODE_LIST)

include_directories(../gen-cpp)

aux_source_directory(../gen-cpp O_GENCPP)


add_executable(${PROJECT_NAME}
    transferclient.h transferclient.cpp
    ${O_GENCPP}
    main.cpp)

#add_executable(${PROJECT_NAME} ${CODE_LIST})

target_link_libraries(${PROJECT_NAME} muduo_base muduo_net rt thriftnb thrift event ssl crypto)

#asan




