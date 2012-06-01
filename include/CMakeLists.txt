configure_file(ColumbusCore.hh.in ColumbusCore.hh)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ColumbusCore.hh Matcher.hh columbus.h DESTINATION include/columbus)
