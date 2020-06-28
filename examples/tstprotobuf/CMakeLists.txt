


add_subdirectory(codec)
add_subdirectory(rpc)
#add_subdirectory(rpcbalancer)
add_subdirectory(rpcbench)

add_subdirectory(resolver)

#if(CARES_INCLUDE_DIR AND CARES_LIBRARY)
#else()
##  add_subdirectory(resolver EXCLUDE_FROM_ALL)
#endif()
