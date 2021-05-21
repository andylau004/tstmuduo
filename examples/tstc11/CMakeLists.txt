



project(tstc11)

set(EXECUTABLE_OUTPUT_PATH ../../)

include_directories(gen-cpp)
aux_source_directory(gen-cpp O_GENCPP)
aux_source_directory(. O_LOCAL_SOURCE)


add_definitions("-DUSE_MAKE_SHARED")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

MESSAGE(STATUS "CMAKE_CXX_FLAGS is: ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME}
    ${O_LOCAL_SOURCE}
    ${O_GENCPP}
    async_thrift_client.h  async_thrift_client.cpp
    threadsepcific.h  threadsepcific.cpp
    boostany.h boostany.cpp
    c11fun.h c11fun.cpp
    tstbacktrace.h tstbacktrace.cpp
    branch_predict.h branch_predict.cpp
    main.cpp)


#target_link_libraries(${PROJECT_NAME} muduo_base muduo_net rt thriftnb thrift event ssl crypto)
target_link_libraries(${PROJECT_NAME} pthread boost_filesystem boost_system boost_thread muduo_base muduo_net rt thriftnb thrift event ssl crypto   )

#asan




