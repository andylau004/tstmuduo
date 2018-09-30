



project(tstconnhash)


set(EXECUTABLE_OUTPUT_PATH ../../)

#aux_source_directory(. CODE_LIST)

add_executable(${PROJECT_NAME}
    tstconnhash.h tstconnhash.cpp
#    util_rbtree.h util_rbtree.cpp
    md5.h md5.cpp
    MD5HashFunc.h MD5HashFunc.cpp
    Node.h Node.cpp
#    conhash_inter.h conhash_inter.cpp
    ConsistentHash.h ConsistentHash.cpp HashFunc.h main.cpp)
#add_executable(${PROJECT_NAME} ${CODE_LIST})

target_link_libraries(${PROJECT_NAME} muduo_base muduo_net rt  ssl crypto)

#asan




