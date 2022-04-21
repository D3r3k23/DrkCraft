[Title](assets/logo_full.png)

[![CMake Build](https://github.com/D3r3k23/DrkCraft/actions/workflows/cmake_build.yaml/badge.svg)](https://github.com/D3r3k23/DrkCraft/actions/workflows/cmake_build.yaml)

Minecraft-based game in C++ created using an OpenGL renderer

* Minecraft<sup>2</sup>??

### Videos
[Youtube](https://www.youtube.com/playlist?list=PLD41ILT9lDJa0fnwzflIxxq_wivyyBtYb)

### Manual
[manual.md](doc/manual.md)

### Images
[Game](media/game.png)

### Dependencies
* C++ 20
* CMake
* [Dear ImGui](https://github.com/ocornut/imgui)
* [EnTT](https://github.com/skypjack/entt)
* [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)
* [{fmt}](https://github.com/fmtlib/fmt)
* [Glad](https://github.com/Dav1dde/glad)
* [GLFW](https://github.com/glfw/glfw)
* [GLM](https://github.com/g-truc/glm)
* [minimp3](https://github.com/lieff/minimp3)
* [Magic Enum](https://github.com/Neargye/magic_enum)
* [Ogg](https://gitlab.xiph.org/xiph/ogg)
* [OpenAL Soft](https://github.com/kcat/openal-soft)
* [RapidJSON](https://github.com/Tencent/rapidjson)
* [spdlog](https://github.com/gabime/spdlog)
* [stb](https://github.com/nothings/stb)
* [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
* [Vorbis](https://github.com/xiph/vorbis)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
* Python tools require Python 3.10

### Development Environment Setup
1. `git clone --recursive https://github.com/D3r3k23/DrkCraft DrkCraft`
2. `cd DrkCraft`
4. `scripts\setup.bat` or `scripts/setup.sh`
5. `python -m drkcraft build`
6. `python -m drkcraft run`
