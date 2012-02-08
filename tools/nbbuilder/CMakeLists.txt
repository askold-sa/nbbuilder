set(NBBuilderSrc
    main.cpp
    ${CMAKE_SOURCE_DIR}/algorithms/step.cpp
    ${CMAKE_SOURCE_DIR}/algorithms/miscfuncs.cpp
    ${CMAKE_SOURCE_DIR}/algorithms/behavior.cpp
    ${CMAKE_SOURCE_DIR}/algorithms/algorithms.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/include/ )

add_executable(NBBuilder ${NBBuilderSrc})

target_link_libraries(NBBuilder ${BOOST_REGEX_LIBRARIES})
