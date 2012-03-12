project(SpeechControl-DesktopControl-Applications)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPDSKAPP_PLUGIN_MINOR_VERSION 0)
set(SPDSKAPP_PLUGIN_MAJOR_VERSION 1)
set(SPDSKAPP_PLUGIN_UUID "bbb53bbb-d879-43e3-b9ba-f24060293488")
set(SPDSKAPP_PLUGIN_VERSION "${SPDSKAPP_PLUGIN_MAJOR_VERSION}.${SPDSKAPP_PLUGIN_MINOR_VERSION}")
set(SPDSKAPP_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPDSKAPP_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPDSKAPP_SRCS "src/*.cpp")
file(GLOB SPDSKAPP_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPDSKAPP_SRCS})
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)
include_directories(${SPDSKAPP_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    "${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS}")

add_library(spchcntrl-applist SHARED ${SPDSKAPP_SRCS})
target_link_libraries(spchcntrl-applist ${SPDSKAPP_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-applist PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPDSKAPP_PLUGIN_VERSION}"
    SOVERSION "${SPDSKAPP_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-applist)
add_dependencies(plugins spchcntrl-applist)

install(FILES "${PROJECT_SOURCE_DIR}/data/bbb53bbb-d879-43e3-b9ba-f24060293488.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})