cmake_minimum_required(VERSION 2.6)

SET(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "postfix applied to debug build of libraries")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "postfix applied to release build of libraries")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

project(featuresgen)

find_package(Boost COMPONENTS filesystem)
find_package(Threads)
if(Boost_FOUND AND Threads_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(featuresgen mainGenerator.cpp)
  target_link_libraries(featuresgen ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  
  add_executable(classificator mainClassificator.cpp KNNClassifier.cpp)
endif()

install(TARGETS featuresgen classificator DESTINATION bin)
