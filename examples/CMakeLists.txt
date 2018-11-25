



set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses -std=c++11" )

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2 -Wno-deprecated -ffunction-sections -W -Wall -pthread -Wno-missing-braces -Wparentheses" )

add_subdirectory(tstclient)
add_subdirectory(tstserver)

add_subdirectory(tstalgor)
add_subdirectory(tstconnhash)
add_subdirectory(tstlru)
#add_subdirectory(tstthrift)
add_subdirectory(transferserver)
add_subdirectory(transferclient)
add_subdirectory(tstthreadpool)
add_subdirectory(tstprotobuff)
add_subdirectory(tstpoll_srv)
add_subdirectory(tstpoll_cli)
add_subdirectory(tstepoll_srv)
add_subdirectory(tstepoll_cli)
add_subdirectory(tstc11)
add_subdirectory(tsttimewheel)
add_subdirectory(tstidleconnection_srv)
add_subdirectory(tstbucketwheel)
add_subdirectory(tsteventfd)




#add_subdirectory(${CMAKE_SOURCE_DIR}/thirdpart/rapidjson)

#if(CARES_INCLUDE_DIR AND CARES_LIBRARY)
#  add_subdirectory(cdns)
#else()
#  add_subdirectory(cdns EXCLUDE_FROM_ALL)
#endif()

#if(CURL_FOUND)
#  add_subdirectory(curl)
#else()
#  add_subdirectory(curl EXCLUDE_FROM_ALL)
#endif()

#if(PROTOBUF_FOUND)
#  add_subdirectory(ace/logging)
#  add_subdirectory(protobuf)
#else()
#  add_subdirectory(ace/logging EXCLUDE_FROM_ALL)
#  add_subdirectory(protobuf EXCLUDE_FROM_ALL)
#endif()

#if(GD_INCLUDE_DIR AND GD_LIBRARY)
#  add_subdirectory(procmon)
#else()
#  add_subdirectory(procmon EXCLUDE_FROM_ALL)
#endif()

