



project(tstconnhash)


set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(tstconnhash main.cpp )
target_link_libraries(tstconnhash muduo_base muduo_net rt  ssl crypto)

#asan




