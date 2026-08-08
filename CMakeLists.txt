cmake_minimum_required(VERSION 3.24)
project(Cobalt VERSION 0.0.1 LANGUAGES CXX C ASM_MASM)

# -- Needed for EASTL --
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -- Vendor libraries --
add_subdirectory(Vendor)

# -- Targets --
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Tests)
