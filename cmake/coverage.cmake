if (CMAKE_BUILD_TYPE MATCHES coverage)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")

  find_program(GCOVR_EXECUTABLE gcovr HINTS ${GCOVR_ROOT} "${GCOVR_ROOT}/bin")
  if (NOT GCOVR_EXECUTABLE)
    message(STATUS "Gcovr binary was not found, can not generate XML coverage info.")
  else ()
    message(STATUS "Gcovr found, can generate XML coverage info.")
    add_custom_target (coverage-xml
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND "${GCOVR_EXECUTABLE}" --exclude="test.*" -x -r "${CMAKE_SOURCE_DIR}" 
      --object-directory=${CMAKE_BINARY_DIR} -o coverage.xml)
  endif()
endif()
