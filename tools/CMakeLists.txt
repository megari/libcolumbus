add_executable(queryindex queryindex.cc)
target_link_libraries(queryindex columbus)

if(GTK3_FOUND)
  include_directories(${GTK3_INCLUDE_DIRS})
  add_executable(singleword singleword.cc)
  target_link_libraries(singleword columbus ${GTK3_LIBRARIES})
  
  add_executable(queryapp queryapp.cc)
  target_link_libraries(queryapp columbus ${GTK3_LIBRARIES})
endif()
