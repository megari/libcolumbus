project(columbus C CXX)
cmake_minimum_required(VERSION 2.8)

if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
   message(FATAL_ERROR "In-tree build attempt detected, aborting. Set your build dir outside your source dir, delete CMakeCache.txt from source root and try again.")
endif()

option(enable_tests "Enable tests.")
option(enable_scalability_tests "Additional scalability tests that are potentially very slow to run." OFF)
option(full_warnings "Full compiler warnings." ON)
option(debug_messages "Print debug messages.")
option(full_unicode "Enable full Unicode support (takes lots of memory).")

if(${full_warnings})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

set(SO_VERSION_MAJOR "0")
set(SO_VERSION_MINOR "1")
set(SO_VERSION_PATCH "0")

set(SO_VERSION "${SO_VERSION_MAJOR}.${SO_VERSION_MINOR}.${SO_VERSION_PATCH}")

# Increment this manually whenever breaking ABI.
# http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html#AEN135
set(ABI_VERSION 0)

# Should be lib64 on some platforms.
set(LIBDIR "lib")

include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

if(full_unicode)
  if(${IS_BIG_ENDIAN})
    set(INTERNAL_ENCODING "UTF-32BE") # Iconv encoding string.
  else()
    set(INTERNAL_ENCODING "UTF-32LE")
  endif()
  set(LETTER_TYPE "uint32_t")
else()
  if(${IS_BIG_ENDIAN})
    set(INTERNAL_ENCODING "UCS-2BE//IGNORE") # Drop everything outside Base Multilingual Plane.
  else()
    set(INTERNAL_ENCODING "UCS-2LE//IGNORE")
  endif()
  set(LETTER_TYPE "uint16_t")
endif()  

# Only required on some platforms.
# If not found, that's ok.
find_library(ICONV_LIBRARIES iconv)

include(FindPkgConfig)
pkg_search_module(GTK3 gtk+-3.0)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

if(${debug_messages})
  add_definitions(-DDEBUG_MESSAGES)
endif()

# Remove this once we go full C++1x and the compiler will shut up.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-variadic-macros")

find_library(icu1 icuuc)
find_library(icu2 icudata)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tools)

if(${enable_tests})
  enable_testing()
  add_subdirectory(test)
endif()
