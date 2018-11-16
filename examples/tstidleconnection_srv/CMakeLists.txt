

project(tstidleconnection_srv)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(tstidleconnection_srv
    tstidleconnection_srv.h tstidleconnection_srv.cpp
    main.cpp)


target_link_libraries(tstidleconnection_srv event muduo_net muduo_base)







