#add_library(muduo_cdns Resolver.cc)
#target_link_libraries(muduo_cdns muduo_net)
#target_link_libraries(muduo_cdns cares)

#install(TARGETS muduo_cdns DESTINATION lib)
#install(FILES Resolver.h DESTINATION include/muduo/cdns)

#add_executable(cdns dns.cc)
#target_link_libraries(cdns muduo_cdns)


project(tstcli)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses -std=c++11" )

set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME} muduo_base muduo_net)
target_link_libraries(${PROJECT_NAME} muduo_base muduo_net pthread boost_system boost_thread muduo_base muduo_net rt thriftnb thrift event ssl crypto z dl )







