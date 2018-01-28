cmake_minimum_required(VERSION 3.0)

include(cmake/hunter.cmake)

project(bzip2stream VERSION 1.0.0)

option(BZIP2STREAM_BUILD_TEST "Builds tests" OFF)

add_library(bzip2stream INTERFACE)

target_compile_definitions(bzip2stream INTERFACE BZ_NO_STDIO)

hunter_add_package(BZip2)
find_package(BZip2 CONFIG REQUIRED)
target_link_libraries(bzip2stream INTERFACE BZip2::bz2)

target_include_directories(bzip2stream INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
  )

if(BZIP2STREAM_BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

# Installation (https://github.com/forexample/package-example) {

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   * PROJECT_VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * TARGETS_EXPORT_NAME
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Targets (INTERFACE):
#   * header location after install: <prefix>/include/bzip2stream/iostream_bz.h
#   * headers can be included by C++ code `#include <bzip2stream/iostream_bz.h>`
install(
    TARGETS bzip2stream
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
#   * include/stdext/path.h -> <prefix>/include/stdext/path.h
install(
    DIRECTORY "bzip2stream" # no trailing slash
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.h"
)

# Config
#   * <prefix>/lib/cmake/bzip2stream/bzip2streamConfig.cmake
#   * <prefix>/lib/cmake/bzip2stream/bzip2streamConfigVersion.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
#   * <prefix>/lib/cmake/bzip2stream/bzip2streamTargets.cmake
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

# }

