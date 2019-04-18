



project(tstlru)


set(EXECUTABLE_OUTPUT_PATH ../../)

#aux_source_directory(. CODE_LIST)

add_executable(${PROJECT_NAME}
    tstlru.h tstlru.cpp 
    LRUCache.h LRUCache.cpp
    main.cpp)
#add_executable(${PROJECT_NAME} ${CODE_LIST})

target_link_libraries(${PROJECT_NAME} muduo_base muduo_net rt  ssl crypto)

#asan




