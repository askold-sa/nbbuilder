project(NBBuilder)
cmake_minimum_required (VERSION 2.6.4)

set(CMAKE_CXX_FLAGS_DEBUG               "-O3 -Wextra -g" )
set(CMAKE_C_FLAGS_DEBUG                 "-O3 -Wextra -g" )

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO      "-O3 -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE             "-O3 -Wextra")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # set here
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

