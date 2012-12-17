set(build_python FALSE)

find_package(Boost 1.49.0 COMPONENTS python)
pkg_search_module(PYTHONLIBS python3)

if(NOT Boost_FOUND)
  message(STATUS "Boost not found, not building Python bindings.")
else()
  if(NOT PYTHONLIBS_FOUND)
    message(STATUS "Python dev libraries not found, not building Python bindings.")
  else()
    message(STATUS ${PYTHONLIBS_VERSION})
    string(SUBSTRING ${PYTHONLIBS_VERSION} 0 1 PYTHON_MAJOR)
    string(SUBSTRING ${PYTHONLIBS_VERSION} 2 1 PYTHON_MINOR)
    if(NOT ${PYTHON_MAJOR} STREQUAL 3)
      message(WARNING "Not building against Python 3. This is unsupported and probably won't work.")
    endif()
    message(STATUS "Found Python version ${PYTHON_MAJOR}.${PYTHON_MINOR}.")

    # Linking against libboost_python does not work with Python 3.
    # Working around this bug:
    # http://public.kitware.com/Bug/view.php?id=12955
    find_library(BOOST_PYTHON_HACK boost_python-py${PYTHON_MAJOR}${PYTHON_MINOR})
    if(NOT BOOST_PYTHON_HACK)
      message(STATUS "Boost.Python hack library not found, not building Python bindings")
    else()
      set(build_python TRUE)
      message(STATUS "Building Python bindings.")
    endif()
  endif()
endif()
