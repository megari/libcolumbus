add_executable(assert_fail AssertFailTest.cc)
add_test(assert_fail assert_fail)
set_tests_properties(assert_fail PROPERTIES WILL_FAIL TRUE)

add_executable(assert_succ AssertSuccTest.cc)
add_test(assert_succ assert_succ)

add_executable(levtrie LevTrieTest.cc)
target_link_libraries(levtrie columbus)
add_test(levtrie levtrie)

add_executable(levindex LevIndexTest.cc)
target_link_libraries(levindex columbus)
add_test(levindex levindex)

add_executable(lev_scalability LevScalabilityTest.cc)
target_link_libraries(lev_scalability columbus)
if(${enable_scalability_tests})
  add_test(lev_scalability lev_scalability /usr/share/dict/words)
endif()

add_executable(custom_error CustomErrorTest.cc)
target_link_libraries(custom_error columbus)
add_test(custom_error custom_error)

add_executable(word WordTest.cc)
target_link_libraries(word columbus)
add_test(word word)

add_executable(wordlist WordListTest.cc)
target_link_libraries(wordlist columbus)
add_test(wordlist wordlist)

add_executable(document DocumentTest.cc)
target_link_libraries(document columbus)
add_test(document document)

add_executable(corpus CorpusTest.cc)
target_link_libraries(corpus columbus)
add_test(corpus corpus)

add_executable(matcher MatcherTest.cc)
target_link_libraries(matcher columbus)
add_test(matcher matcher)

add_executable(matchresults MatchResultsTest.cc)
target_link_libraries(matchresults columbus)
add_test(matchresults matchresults)

add_executable(helpers HelpersTest.cc)
target_link_libraries(helpers columbus)
add_test(helpers helpers)

add_executable(create_performance CreatePerformanceTest.cc)
target_link_libraries(create_performance columbus)

add_executable(indexweights IndexWeightsTest.cc)
target_link_libraries(indexweights columbus)
add_test(indexweights indexweights)
