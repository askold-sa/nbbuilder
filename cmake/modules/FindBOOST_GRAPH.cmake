if (BOOST_GRAPH_INCLUDE_DIRS)
  set(BOOST_GRAPH_FOUND TRUE)
else (BOOST_GRAPH_INCLUDE_DIRS)
  find_path(BOOST_GRAPH_INCLUDE_DIRS
    NAMES
      boost/graph/
    PATHS
      /usr/include
      /usr/local/include
  )

  if (BOOST_GRAPH_INCLUDE_DIRS)
     set(BOOST_GRAPH_FOUND TRUE)
  endif (BOOST_GRAPH_INCLUDE_DIRS)

  if (BOOST_GRAPH_FOUND)
    if (NOT BOOST_GRAPH_FIND_QUIETLY)
      message(STATUS "Found boost/graph: ${BOOST_GRAPH_INCLUDE/DIRS}")
    endif (NOT BOOST_GRAPH_FIND_QUIETLY)
  else (BOOST_GRAPH_FOUND)
    if (BOOST_GRAPH_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find boost/graph")
    endif (BOOST_GRAPH_FIND_REQUIRED)
  endif (BOOST_GRAPH_FOUND)

  mark_as_advanced(BOOST_GRAPH_INCLUDE_DIRS)

endif (BOOST_GRAPH_INCLUDE_DIRS)
