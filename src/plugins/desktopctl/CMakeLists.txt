cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_SOURCE_DIR}/src/app/services")

add_subdirectory(apps)
add_subdirectory(mpris)
add_subdirectory(syscntrl)
