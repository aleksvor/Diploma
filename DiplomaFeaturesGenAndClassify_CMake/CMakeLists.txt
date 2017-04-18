cmake_minimum_required(VERSION 2.6)
project(featuresgen)

find_package(Boost COMPONENTS filesystem)
find_package(Threads)
if(Boost_FOUND AND Threads_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(featuresgen mainGenerator.cpp)
  target_link_libraries(featuresgen ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

install(TARGETS featuresgen RUNTIME DESTINATION bin)
