add_executable(Tests
    Source/Core/UUID.cpp
)

target_link_libraries(Tests PRIVATE
    Engine
    Catch2::Catch2WithMain
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|Intel")
    target_compile_options(Tests PRIVATE -fno-exceptions)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(Tests PRIVATE /EHsc-)
endif()