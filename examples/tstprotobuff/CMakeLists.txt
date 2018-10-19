



project(tstprotobuf)


set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(${PROJECT_NAME}
    pbwork.h pbwork.cpp
    addressbook.pb.cc addressbook.pb.h
    addressbook.proto
    main.cpp)


target_link_libraries(${PROJECT_NAME} protobuf gtest muduo_base muduo_net rt thriftnb thrift event ssl crypto)





