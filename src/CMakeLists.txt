add_library(columbus SHARED
LevenshteinIndex.cc
MatchRow.cc
IndexMatches.cc
ErrorValues.cc
Word.cc
)

#target_link_libraries(columbus ${ICONV_LIBRARIES}

set_target_properties(columbus PROPERTIES VERSION ${SO_VERSION} SOVERSION ${ABI_VERSION})

install(
  TARGETS columbus
  ARCHIVE DESTINATION ${LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
)
