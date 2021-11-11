# DrkCraft

[![CMake Build](https://github.com/D3r3k23/DrkCraft/actions/workflows/cmake_build.yaml/badge.svg)](https://github.com/D3r3k23/DrkCraft/actions/workflows/cmake_build.yaml)

Minecraft clone created using an OpenGL renderer

* Currently only built for Windows

### Dependencies
* C++ 20
* CMake
* Python (building)

### Libraries
* EnTT
* Glad
* GLFW
* GLM
* Dear ImGui
* spdlog

### Build Instructions
1. `git clone --recursive https://github.com/D3r3k23/DrkCraft DrkCraft`
2. `cd DrkCraft`
3. `scripts\gen_glad.bat`
4. `scripts\build.bat`
5. `scripts\run.bat`
