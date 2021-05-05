

project(tstmemleak)


set(EXECUTABLE_OUTPUT_PATH ../../)

include_directories(/usr/include)

link_directories(/usr/lib)


add_executable(${PROJECT_NAME}
    nmx_palloc.cpp nmx_palloc.h
    nmx_alloc.cpp nmx_alloc.h
    ngxmempool.h ngxmempool.cpp
    tst-memcheck.cpp
    tst-meml.h tst-meml.cpp
    tstperf.h tstperf.cpp
    main.cpp)

# TARGET_LINK_OPTIONS(${PROJECT_NAME} PUBLIC -rdynamic
#     -Wl,--wrap=malloc
#     -Wl,--wrap=calloc
#     -Wl,--wrap=realloc
#     -Wl,--wrap=strdup
#     -Wl,--wrap=open
#     -Wl,--wrap=fopen
#     -Wl,--wrap=close
#     -Wl,--wrap=fclose
#     -Wl,--wrap=fstat
#     -Wl,--wrap=fseek
#     -Wl,--wrap=ftell
#     -Wl,--wrap=fread
#     -Wl,--wrap=fwrite
#     -Wl,--wrap=read
#     -Wl,--wrap=write
#     -Wl,--wrap=mmap)



# TARGET_LINK_OPTIONS(${PROJECT_NAME} PUBLIC -rdynamic
#     -Wl,--wrap=malloc
#     -Wl,--wrap=calloc
#     -Wl,--wrap=realloc
#     -Wl,--wrap=strdup
#     -Wl,--wrap=open
#     -Wl,--wrap=fopen
#     -Wl,--wrap=close
#     -Wl,--wrap=fclose
#     -Wl,--wrap=fstat
#     -Wl,--wrap=fseek
#     -Wl,--wrap=ftell
#     -Wl,--wrap=fread
#     -Wl,--wrap=fwrite
#     -Wl,--wrap=read
#     -Wl,--wrap=write
#     -Wl,--wrap=mmap)


target_link_libraries(${PROJECT_NAME} muduo_net muduo_base leaktracer dl)







