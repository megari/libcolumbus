include_directories(${Boost_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})

add_library(_columbus_ext _columbus.cc)
target_link_libraries(_columbus_ext ${COL_LIB_BASENAME} ${BOOST_PYTHON_HACK} ${PYTHON_LIBRARIES})