# Cobalt Engine
Cobalt Engine is a simple 2D game engine written in C++, using OpenGL as its graphics API.
> Note: The engine is currently in an early stage and not yet capable of producing complete games.

## Platform support
- Windows
> Note: Linux support may be added in the future.

## 0.1 Roadmap
The primary goal for version 0.1 is to make the engine capable of creating a simple game through its editor.
- [ ] Asset management
- [ ] Project launcher
- [ ] Scripting
- [ ] Audio
- [ ] Physics
- [ ] Runtime GUI
- [ ] Building and Packaging

## Building
#### Requirements
- **CMake** ≥ 3.24
- **C++ Compiler** supporting **C++23**
  - **MSVC** (Visual Studio 2022)
  - **Gcc** ≥ 11
  - **Clang** ≥ 18
- Ninja *(optional)*

Clone the repository
``` sh
git clone --recursive https://github.com/Resongeo/Cobalt-Engine
```
Change directory
``` sh
cd Cobalt-Engine
```

#### If you’re building from an IDE, use its built-in CMake tools to configure and build the project. For manual builds, run the following commands:

Create a build directory and generate the build files:
``` sh
cmake -B build
```
Build the project:
``` sh
cmake --build build
```

## Dependencies
- [EnTT](https://github.com/skypjack/entt) - Fast and reliable ECS
- [Glad](https://github.com/Dav1dde/glad) - OpenGL function loader
- [Glm](https://github.com/g-truc/glm) - Mathematics library for graphics applications
- [Dear ImGui](https://github.com/ocornut/imgui) - Immediate-mode GUI library for the editor
- [Toml++](https://github.com/marzer/tomlplusplus) - TOML config parser and serializer
- [Catch 2](https://github.com/catchorg/Catch2) - Unit testing framework
