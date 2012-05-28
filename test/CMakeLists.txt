add_executable(assert_fail assert_fail_test.cc)
add_test(assert_fail assert_fail)
set_tests_properties(assert_fail PROPERTIES WILL_FAIL TRUE)

add_executable(assert_succ assert_succ_test.cc)
add_test(assert_succ assert_succ)

add_executable(levtrie levtrie_test.cc)
target_link_libraries(levtrie columbus)
add_test(levtrie levtrie)

add_executable(levindex levindex_test.cc)
target_link_libraries(levindex columbus)
add_test(levindex levindex)

add_executable(lev_scalability lev_scalability_test.cc)
target_link_libraries(lev_scalability columbus)
if(${enable_scalability_tests})
  add_test(lev_scalability lev_scalability /usr/share/dict/words)
endif()

add_executable(custom_error custom_error_test.cc)
target_link_libraries(custom_error columbus)
add_test(custom_error custom_error)

add_executable(word word_test.cc)
target_link_libraries(word columbus)
add_test(word word)

add_executable(wordlist wordlist_test.cc)
target_link_libraries(wordlist columbus)
add_test(wordlist wordlist)

add_executable(document document_test.cc)
target_link_libraries(document columbus)
add_test(document document)
