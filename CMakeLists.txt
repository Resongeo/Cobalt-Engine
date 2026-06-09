cmake_minimum_required(VERSION 3.24)
project(Cobalt VERSION 0.0.1 LANGUAGES CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -- Change subsystem to windows on Release --
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
	set(CMAKE_WIN32_EXECUTABLE ON)
endif ()

# -- Vendor libraries --
add_subdirectory(Vendor)

# -- Targets --
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Tests)
