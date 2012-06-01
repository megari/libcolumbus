configure_file(ColumbusCore.hh.in ColumbusCore.hh)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ColumbusCore.hh Matcher.hh columbus.h DESTINATION include/columbus)

# Build and install a pkg-config file
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${prefix}/bin)
set(libdir ${prefix}/${LIBDIR})
set(includedir ${prefix}/include)
configure_file(columbus.pc.in columbus.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/columbus.pc DESTINATION ${LIBDIR}/pkgconfig)
