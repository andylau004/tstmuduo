#add_library(muduo_cdns Resolver.cc)
#target_link_libraries(muduo_cdns muduo_net)
#target_link_libraries(muduo_cdns cares)

#install(TARGETS muduo_cdns DESTINATION lib)
#install(FILES Resolver.h DESTINATION include/muduo/cdns)

#add_executable(cdns dns.cc)
#target_link_libraries(cdns muduo_cdns)


project(tstsrv)



#set(CXX_FLAGS
#    -g
#    # -DVALGRIND
#    # -DMUDUO_STD_STRING
#    -DCHECK_PTHREAD_RETURN_VALUE
#    -D_FILE_OFFSET_BITS=64
#    -Wall
#    -Wextra
#    -Werror
#    -Wconversion
#    -Wno-unused-parameter
#    -Wold-style-cast
#    -Woverloaded-virtual
#    -Wpointer-arith
#    -Wshadow
#    -Wwrite-strings
#    -march=native
#    # -MMD
#    # -std=c++0x
#    -std=c++11
#    -rdynamic
#    )
set(EXECUTABLE_OUTPUT_PATH ../../)

add_executable(tstsrv main.cpp parent.cpp child.cpp pubhead.h pubhead.cpp stockfactory.cpp booststockfactory.cpp)
target_link_libraries(tstsrv muduo_base muduo_net rt boost_thread boost_system  boost_filesystem)







