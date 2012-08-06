add_executable(queryindex queryindex.cc)
target_link_libraries(queryindex columbus)

if(GTK3_FOUND)
  include_directories(${GTK3_INCLUDE_DIRS})
  add_executable(singleword singleword.cc)
  target_link_libraries(singleword columbus ${GTK3_LIBRARIES})
  
  add_executable(queryapp queryapp.cc)
  target_link_libraries(queryapp columbus ${GTK3_LIBRARIES})

  add_executable(hudtest hudtest.cc)
  target_link_libraries(hudtest columbus ${GTK3_LIBRARIES})

  add_executable(sctest sctest.cc)
  target_link_libraries(sctest columbus ${GTK3_LIBRARIES})

  install(
    TARGETS singleword queryapp
    ARCHIVE DESTINATION ${LIBDIR}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIBDIR}
  )
endif()

if(icu1)
  add_executable(lowertable lowertable.cc)
  target_link_libraries(lowertable ${icu1} ${icu2})
endif()
