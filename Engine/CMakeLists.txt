add_library(Engine STATIC
    Source/Engine/Core/Types.hpp
    Source/Engine/Core/Application.cpp
    Source/Engine/Core/Application.hpp
)

target_include_directories(Engine PUBLIC
    Source
)