cmake_minimum_required(VERSION 3.24)
project(Cobalt)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(ASM_MASM)

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
