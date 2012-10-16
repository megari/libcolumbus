add_library(columbus SHARED
LevenshteinIndex.cc
IndexMatches.cc
ErrorValues.cc
Word.cc
ColumbusHelpers.cc
ColumbusSlow.cc
WordList.cc
Document.cc
Corpus.cc
Matcher.cc
MatchResults.cc
IndexWeights.cc
MatcherStatistics.cc
WordStore.cc
ColumbusCAPI.cc
ErrorMatrix.cc
ResultFilter.cc
)

if(ICONV_LIBRARIES)
  target_link_libraries(columbus ${ICONV_LIBRARIES})
endif()
target_link_libraries(columbus ${icu1} ${icu2})

set_target_properties(columbus PROPERTIES VERSION ${SO_VERSION} SOVERSION ${ABI_VERSION})

add_pch(pch/columbus_pch.hh columbus)

install(
  TARGETS columbus
  ARCHIVE DESTINATION ${LIBDIR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBDIR}
)
