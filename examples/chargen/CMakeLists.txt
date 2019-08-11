

project(chargen)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(chargen
    chargenfun.h chargenfun.cpp
    main.cpp)

target_link_libraries( ${PROJECT_NAME}
    pthread boost_system boost_thread muduo_base muduo_net_cpp11
    rt thriftnb thrift event ssl crypto )








