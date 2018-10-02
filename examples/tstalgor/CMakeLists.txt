#add_library(muduo_cdns Resolver.cc)
#target_link_libraries(muduo_cdns muduo_net)
#target_link_libraries(muduo_cdns cares)

#install(TARGETS muduo_cdns DESTINATION lib)
#install(FILES Resolver.h DESTINATION include/muduo/cdns)

#add_executable(cdns dns.cc)
#target_link_libraries(cdns muduo_cdns)


project(tstalgo)


set(EXECUTABLE_OUTPUT_PATH ../../)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses -std=c++11" )


add_executable(tstalgo main.cpp boost_use_1.h boost_use_1.cpp trie_tree.h trie_tree.cpp lru.h lru.cpp classfun.h classfun.cpp tstBinaryTree.cpp quicksort.cpp tstList.cpp tstList.h complexdata.cpp complexdata.h mergesort.cpp mergesort.h heapsort.cpp heapsort.h externMergSort.cpp externMergSort.h hashfun.cpp hashfun.h linkedlistopr.h linkedlistopr.cpp stackopr.h stackopr.cpp)
target_link_libraries(tstalgo muduo_base muduo_net rt  ssl crypto)

#asan




