





project(tstprotobuf)


set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(${PROJECT_NAME}
    sylar.proto
    sylar.pb.h sylar.pb.cc
    parsepbfile.h parsepbfile.cpp
    pbwork.h pbwork.cpp
    addressbook.pb.cc addressbook.pb.h
    codec.h codec.cpp codec_test.cpp
    query.pb.cc  query.pb.h
    addressbook.proto query.proto
    main.cpp)


target_link_libraries(${PROJECT_NAME} z protobuf gtest muduo_base muduo_net rt thriftnb thrift event ssl crypto)





