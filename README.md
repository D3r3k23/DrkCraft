```
$$$$$$$\            $$\        $$$$$$\                       $$$$$$\    $$
$$  __$$\           $$ |      $$  __$$\                     $$  __$$\   $$ |
$$ |  $$ | $$$$$$\  $$ |  $$\ $$ /  \__| $$$$$$\   $$$$$$\  $$ /  \__|$$$$$$
$$ |  $$ |$$  __$$\ $$ | $$  |$$ |      $$  __$$\  \____$$\ $$$$\     \_$$  _|
$$ |  $$ |$$ |  \__|$$$$$$  / $$ |      $$ |  \__| $$$$$$$ |$$  _|      $$ |
$$ |  $$ |$$ |      $$  _$$<  $$ |  $$\ $$ |      $$  __$$ |$$ |        $$ |$$\
$$$$$$$  |$$ |      $$ | \$$\ \$$$$$$  |$$ |      \$$$$$$$ |$$ |        \$$$$  |
\_______/ \__|      \__|  \__| \______/ \__|       \_______|\__|         \____/
```

[![Build](https://github.com/D3r3k23/DrkCraft/actions/workflows/build.yaml/badge.svg)](https://github.com/D3r3k23/DrkCraft/actions/workflows/build.yaml)

#### Minecraft-based game in C++ created using an OpenGL renderer

* Minecraft<sup>2</sup>??

## Videos
[Youtube](https://www.youtube.com/playlist?list=PLD41ILT9lDJa0fnwzflIxxq_wivyyBtYb)

## Manual
[manual.md](doc/manual.md)

## Images
[Game](media/game.png)

## Dependencies

* **C++ 20**
* **CMake**
* **Python 3.10**
* [**Backward**](https://github.com/bombela/backward-cpp)
* [**Dear ImGui**](https://github.com/ocornut/imgui)
* [**EnTT**](https://github.com/skypjack/entt)
* [**FastNoiseLite**](https://github.com/Auburn/FastNoiseLite)
* [**{fmt}**](https://github.com/fmtlib/fmt)
* [**Glad**](https://github.com/Dav1dde/glad)
* [**GLFW**](https://github.com/glfw/glfw)
* [**GLM**](https://github.com/g-truc/glm)
* [**minimp3**](https://github.com/lieff/minimp3)
* [**Magic Enum**](https://github.com/Neargye/magic_enum)
* [**Ogg**](https://gitlab.xiph.org/xiph/ogg)
* [**OpenAL Soft**](https://github.com/kcat/openal-soft)
* [**RapidJSON**](https://github.com/Tencent/rapidjson)
* [**spdlog**](https://github.com/gabime/spdlog)
* [**stb**](https://github.com/nothings/stb)
* [**tinyobjloader**](https://github.com/tinyobjloader/tinyobjloader)
* [**Vorbis**](https://github.com/xiph/vorbis)
* [**yaml-cpp**](https://github.com/jbeder/yaml-cpp)

## Build Instructions
0. `$ sudo apt install xorg-dev` (Debian/Ubuntu/Linux Mint)
1. `$ git clone --recursive https://github.com/D3r3k23/DrkCraft DrkCraft`
2. `$ cd DrkCraft`
3.
   a. `$ scripts\setup.bat` (Windows)
   b. `$ source scripts/setup.sh` (Linux)
5. `(.venv) $ python -m drkcraft build`
6. `(.venv) $ python -m drkcraft run`

### Or invoke CMake and run directly:

#### Build
`$ cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug`
`$ cmake --build build --config Debug`

#### Run
`$ build/bin/Debug/DrkCraft` (Linux)
`$ build\bin\Debug\DrkCraft.exe` (Windows)

### License
* LICENSE.txt
* lib/LICENSE.toml
* assets/...
