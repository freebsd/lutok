cmake_minimum_required(VERSION 3.31)
project(lutok VERSION 0.6.3 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default installation prefix to /usr/local for FreeBSD and macOS
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(CMAKE_SYSTEM_NAME MATCHES "FreeBSD" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
        set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation prefix" FORCE)
    endif()
endif()

# Include GNUInstallDirs for standard installation paths
include(GNUInstallDirs)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find Lua using CMake's built-in module
find_package(Lua 5.3 REQUIRED)

# Library sources
set(LUTOK_SOURCES
    src/c_gate.cpp
    src/debug.cpp
    src/exceptions.cpp
    src/operations.cpp
    src/stack_cleaner.cpp
    src/state.cpp
)

# Public headers
file(GLOB LUTOK_PUBLIC_HEADERS "include/lutok/*.hpp" "include/lutok/*.ipp")

# Create shared library
add_library(lutok SHARED ${LUTOK_SOURCES})
set_target_properties(lutok PROPERTIES
    VERSION 3.0.0
    SOVERSION 3
    PUBLIC_HEADER "${LUTOK_PUBLIC_HEADERS}"
)

# Include directories
target_include_directories(lutok
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${LUA_INCLUDE_DIR}
)

# Link Lua
target_link_libraries(lutok PUBLIC ${LUA_LIBRARIES})

# Developer mode - auto-enabled when building from git
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    option(DEVELOPER_MODE "Enable developer mode (strict warnings, -Werror)" ON)
    if(DEVELOPER_MODE)
        message(STATUS "Building from git repository - developer mode auto-enabled")
    endif()
else()
    option(DEVELOPER_MODE "Enable developer mode (strict warnings, -Werror)" OFF)
endif()

if(DEVELOPER_MODE)
    message(STATUS "Developer mode enabled - treating warnings as errors")
    target_compile_options(lutok PRIVATE
        -Wall
        -Wcast-qual
        -Wextra
        -Wpointer-arith
        -Wredundant-decls
        -Wreturn-type
        -Wshadow
        -Wsign-compare
        -Wswitch
        -Wwrite-strings
        -Wctor-dtor-privacy
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wreorder
        -Wsign-promo
        -g
        -Werror
    )
    target_compile_definitions(lutok PRIVATE _FORTIFY_SOURCE=2)
else()
    target_compile_definitions(lutok PRIVATE NDEBUG)
endif()

# Optional: Build examples
option(BUILD_EXAMPLES "Build example programs" OFF)
if(BUILD_EXAMPLES)
    add_executable(example_bindings examples/bindings.cpp)
    target_link_libraries(example_bindings PRIVATE lutok)

    add_executable(example_hello examples/hello.cpp)
    target_link_libraries(example_hello PRIVATE lutok)

    add_executable(example_interpreter examples/interpreter.cpp)
    target_link_libraries(example_interpreter PRIVATE lutok)

    add_executable(example_raii examples/raii.cpp)
    target_link_libraries(example_raii PRIVATE lutok)
endif()

# Optional: Build documentation with Doxygen
option(BUILD_DOCS "Build API documentation with Doxygen" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        # Add custom target for building documentation
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )

        # Optionally install documentation
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api-docs/html
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            OPTIONAL
        )
    else()
        message(WARNING "Doxygen not found - documentation will not be built")
    endif()
endif()

# Optional: Build tests
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    find_package(ATF QUIET COMPONENTS CXX)
    if(ATF_FOUND)
        enable_testing()

        # Helper macro for creating test executables
        macro(add_lutok_test test_name)
            add_executable(${test_name} tests/${test_name}.cpp)
            target_link_libraries(${test_name} PRIVATE
                lutok
                ATF::CXX
            )
            add_test(NAME ${test_name} COMMAND ${test_name})
        endmacro()

        # Add test executables
        add_lutok_test(c_gate_test)
        add_lutok_test(debug_test)
        add_lutok_test(exceptions_test)
        add_lutok_test(operations_test)
        add_lutok_test(stack_cleaner_test)
        add_lutok_test(state_test)
    else()
        message(STATUS "ATF not found - tests will not be built")
    endif()
endif()

# Installation
install(TARGETS lutok
    EXPORT LutokTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lutok
)

# Install CMake config files for find_package(Lutok)
install(EXPORT LutokTargets
    FILE LutokTargets.cmake
    NAMESPACE Lutok::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lutok
)

include(CMakePackageConfigHelpers)

# Generate LutokConfig.cmake
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LutokConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/LutokConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lutok
)

# Generate LutokConfigVersion.cmake
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/LutokConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/LutokConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/LutokConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Lutok
)

# Install example files
install(FILES
    examples/CMakeLists.txt
    examples/bindings.cpp
    examples/hello.cpp
    examples/interpreter.cpp
    examples/raii.cpp
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples
)
