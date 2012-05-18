add_library(columbus SHARED LevenshteinIndex.cc)

set_target_properties(columbus PROPERTIES VERSION ${SO_VERSION} SOVERSION ${ABI_VERSION})

install(
  TARGETS columbus
  ARCHIVE DESTINATION ${LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
)
