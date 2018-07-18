#add_library(muduo_cdns Resolver.cc)
#target_link_libraries(muduo_cdns muduo_net)
#target_link_libraries(muduo_cdns cares)

#install(TARGETS muduo_cdns DESTINATION lib)
#install(FILES Resolver.h DESTINATION include/muduo/cdns)

#add_executable(cdns dns.cc)
#target_link_libraries(cdns muduo_cdns)


project(tstsrv)


set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(tstsrv main.cpp parent.cpp child.cpp pubhead.h pubhead.cpp)
target_link_libraries(tstsrv muduo_base muduo_net)







