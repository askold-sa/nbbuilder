set(NBBuilderSrc
    main.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/include/ )

add_executable(NBBuilder ${NBBuilderSrc})
