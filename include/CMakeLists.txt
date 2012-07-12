configure_file(ColumbusCore.hh.in ColumbusCore.hh)
install(FILES
${CMAKE_CURRENT_BINARY_DIR}/ColumbusCore.hh
Matcher.hh
MatchResults.hh
columbus.h
columbus.hh
Word.hh
Corpus.hh
ErrorValues.hh
Document.hh
ColumbusHelpers.hh
IndexWeights.hh
DESTINATION include/columbus)

# Build and install a pkg-config file
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${prefix}/bin)
set(libdir ${prefix}/${LIBDIR})
set(includedir ${prefix}/include)
configure_file(libcolumbus.pc.in libcolumbus.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcolumbus.pc DESTINATION ${LIBDIR}/pkgconfig)
