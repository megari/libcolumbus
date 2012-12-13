include_directories(${Boost_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})

# This is probably architecture specific.
set(python_lib_name "_columbus.cpython-${PYTHON_MAJOR}${PYTHON_MINOR}mu")

add_library(_columbus_ext SHARED _columbus.cc)
target_link_libraries(_columbus_ext ${COL_LIB_BASENAME} ${BOOST_PYTHON_HACK} ${PYTHON_LIBRARIES})
set_target_properties(_columbus_ext PROPERTIES OUTPUT_NAME ${python_lib_name} PREFIX "")

add_pch(pch/colpython_pch.hh _columbus_ext)

install(TARGETS _columbus_ext DESTINATION ${PYTHONDIR})
install(FILES columbus.py DESTINATION ${PYTHONDIR})
