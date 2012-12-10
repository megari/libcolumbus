set(build_python FALSE)

find_package(Boost 1.49.0 COMPONENTS python)

if(NOT Boost_FOUND)
  message(STATUS "Boost not found, not building Python bindings.")
else()
  find_package(PythonLibs)
  if(NOT PYTHONLIBS_FOUND)
    message(STATUS "Python dev libraries not found, not building Python bindings.")
  else()
    string(SUBSTRING ${PYTHONLIBS_VERSION_STRING} 0 1 PYTHON_MAJOR)
    string(SUBSTRING ${PYTHONLIBS_VERSION_STRING} 2 1 PYTHON_MINOR)
    if(NOT ${PYTHON_MAJOR} STREQUAL 3)
      message(WARNING "Not building against Python 3. This is unsupported and probably won't work.")
    endif()
    message(STATUS "Building against Python version ${PYTHON_MAJOR}.${PYTHON_MINOR}.")

    # Linking against libboost_python does not work with Python 3.
    # Working around this bug:
    # http://public.kitware.com/Bug/view.php?id=12955
    find_library(BOOST_PYTHON_HACK boost_python-py${PYTHON_MAJOR}${PYTHON_MINOR})
    if(NOT BOOST_PYTHON_HACK)
      message(STATUS "Boost.Python hack library not found, not building Python bindings")
    else()
      set(build_python TRUE)
    endif()
  endif()
endif()
