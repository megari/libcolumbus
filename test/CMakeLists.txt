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
add_test(lev_scalability lev_scalability)
