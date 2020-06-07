

#include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/thirdpart)

#file(GLOB evpp_SRCS *.cc */*.cc *.h */*.h)

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
add_subdirectory(chargen)

add_subdirectory(tst_et)
add_subdirectory(tst_lt)
add_subdirectory(tst_memleak)

add_subdirectory(tst-tw-timer)

add_subdirectory(tst_emplaceback)

add_subdirectory(tst_rdshamem)
add_subdirectory(tst_wrshamem)
add_subdirectory(tstskiplist)


#add_subdirectory(${CMAKE_SOURCE_DIR}/thirdpart/rapidjson)

add_subdirectory(cdns)
#if(CARES_INCLUDE_DIR AND CARES_LIBRARY)
#
#else()
#  add_subdirectory(cdns EXCLUDE_FROM_ALL)
#endif()

#if(CURL_FOUND)
#  add_subdirectory(curl)
#else()
#  add_subdirectory(curl EXCLUDE_FROM_ALL)
#endif()

add_subdirectory(tstprotobuf)
if(PROTOBUF_FOUND)
#  add_subdirectory(ace/logging)
#  add_subdirectory(protobuf)
else()
#  add_subdirectory(ace/logging EXCLUDE_FROM_ALL)
#  add_subdirectory(protobuf EXCLUDE_FROM_ALL)
#  add_subdirectory(protobuf)
endif()

#if(GD_INCLUDE_DIR AND GD_LIBRARY)
#  add_subdirectory(procmon)
#else()
#  add_subdirectory(procmon EXCLUDE_FROM_ALL)
#endif()


##add_library(evpp SHARED ${evpp_SRCS})
##target_link_libraries(evpp ${DEPENDENT_LIBRARIES})

## boost lockfree queue
##add_library(evpp_boost SHARED ${evpp_SRCS})
##add_library(evpp_boost_static STATIC ${evpp_SRCS})
##target_compile_definitions(evpp_boost PRIVATE -DH_HAVE_BOOST=1)
##target_compile_definitions(evpp_boost_static PRIVATE -DH_HAVE_BOOST=1)
##target_link_libraries(evpp_boost        ${DEPENDENT_LIBRARIES})
##target_link_libraries(evpp_boost_static ${DEPENDENT_LIBRARIES})

## https://github.com/cameron314/concurrentqueue
#add_library(evpp_concurrentqueue SHARED ${evpp_SRCS})
#add_library(evpp_concurrentqueue_static STATIC ${evpp_SRCS})
#target_compile_definitions(evpp_concurrentqueue PRIVATE -DH_HAVE_CAMERON314_CONCURRENTQUEUE=1)
#target_compile_definitions(evpp_concurrentqueue_static PRIVATE -DH_HAVE_CAMERON314_CONCURRENTQUEUE=1)
#target_link_libraries(evpp_concurrentqueue        ${DEPENDENT_LIBRARIES})
#target_link_libraries(evpp_concurrentqueue_static ${DEPENDENT_LIBRARIES})

##set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
##include (utils)
##include (packages)
#set_target_properties (
##    evpp evpp_boost evpp_concurrentqueue PROPERTIES
#    evpp_concurrentqueue PROPERTIES
#    VERSION     "${PACKAGE_VERSION}"
#    SOVERSION   "${PACKAGE_SOVERSION}"
#)

#install (
##    TARGETS evpp evpp_static evpp_lite_static evpp_boost evpp_boost_static evpp_concurrentqueue evpp_concurrentqueue_static
#    TARGETS  evpp_concurrentqueue evpp_concurrentqueue_static
#  EXPORT ${PACKAGE_NAME}
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib)

