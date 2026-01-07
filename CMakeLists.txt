cmake_minimum_required(VERSION 3.16)
project(izl VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "izl: this project can only be built on Linux.")
endif()

# Options
option(IZL_BUILD_SHARED "Build izl as shared library" OFF)
option(IZL_ENABLE_WARNINGS "Enable extra compiler warnings" ON)

# Auto-discover sources
file(GLOB_RECURSE IZL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)
if(NOT IZL_SOURCES)
    message(FATAL_ERROR "izl: no source files found in src/")
endif()

# Create target
if(IZL_BUILD_SHARED)
    add_library(izl SHARED ${IZL_SOURCES})
else()
    add_executable(izl ${IZL_SOURCES})
endif()

# Auto-discover include directories (include/ and any header dirs under src/)
set(IZL_INCLUDE_DIRS "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    list(APPEND IZL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()

file(GLOB_RECURSE _HDRS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
foreach(_hdr IN LISTS _HDRS)
    get_filename_component(_dir "${_hdr}" DIRECTORY)
    list(APPEND IZL_INCLUDE_DIRS "${_dir}")
endforeach()
list(REMOVE_DUPLICATES IZL_INCLUDE_DIRS)

if(IZL_INCLUDE_DIRS)
    target_include_directories(izl PUBLIC ${IZL_INCLUDE_DIRS})
endif()
# Ensure installed targets reference 'include'
target_include_directories(izl PUBLIC $<INSTALL_INTERFACE:include>)

# Optional warnings
if(IZL_ENABLE_WARNINGS)
    target_compile_options(izl PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
endif()

# Install rules
install(TARGETS izl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

message(STATUS "izl: shared=${IZL_BUILD_SHARED} warnings=${IZL_ENABLE_WARNINGS} platform=${CMAKE_SYSTEM_NAME}")