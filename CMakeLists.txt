#### TERMNOID CMAKE FILE
cmake_minimum_required(VERSION 3.1)
project(termnoid)

## PROJECT FILES
include_directories(${CMAKE_SOURCE_DIR}/inc)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

## EXECUTABLE
add_executable(${PROJECT_NAME} ${PROJECT_SRC})

## LIBRARIES
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  PKG_SEARCH_MODULE(NCURSES REQUIRED ncurses)

  target_include_directories(${PROJECT_NAME} PUBLIC ${NCURSES_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${NCURSES_LIBRARIES})
else()
  message(STATUS "ERROR: pkg-config is not installed on this system and is required to compile termnoid.")
endif()

## FLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -DDEBUG_BUILD")
