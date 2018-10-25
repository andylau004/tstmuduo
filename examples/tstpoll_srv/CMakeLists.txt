#add_library(muduo_cdns Resolver.cc)
#target_link_libraries(muduo_cdns muduo_net)
#target_link_libraries(muduo_cdns cares)

#install(TARGETS muduo_cdns DESTINATION lib)
#install(FILES Resolver.h DESTINATION include/muduo/cdns)

#add_executable(cdns dns.cc)
#target_link_libraries(cdns muduo_cdns)


project(tstpoll_srv)


set(EXECUTABLE_OUTPUT_PATH ../../)



add_executable(tstpoll_srv
    tstpoll_srv.h tstpoll_srv.cpp
    main.cpp)


target_link_libraries(tstpoll_srv event  muduo_net muduo_base)







