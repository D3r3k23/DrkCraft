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

[![Build](https://img.shields.io/github/workflow/status/D3r3k23/DrkCraft/Build?style=plastic)](https://github.com/D3r3k23/DrkCraft/actions/workflows/build.yaml)

#### Minecraft-based game in C++ created using an OpenGL renderer

* Minecraft<sup>2</sup>??

![Game](/media/game.png)

***

## Videos
[Youtube](https://www.youtube.com/playlist?list=PLD41ILT9lDJa0fnwzflIxxq_wivyyBtYb)

## Gallery
[gallery](/gallery)

## Worlds
[worlds](/worlds)

## Manual
[manual.md](/doc/manual.md)

# Development

## Install prerequisites and clone repository
1. `$ sudo apt install xorg-dev` (Debian/Ubuntu/Linux Mint)
2. `$ git clone --recursive https://github.com/D3r3k23/DrkCraft DrkCraft`
3. `$ cd DrkCraft`

## Python Tools

### Setup Python environment
* `$ source scripts/setup.sh` (Linux)
* `$ scripts\setup.bat` (Windows)

### Usage
`python -m drkcraft [-h] [-v] cmd [-h] [cmd options]`

### Build
`(.venv) $ python -m drkcraft build`
* With no options specified, defaults to a Debug build with profiling and dev mode enabled

### Run
`(.venv) $ python -m drkcraft run`

### View other commands
`(.venv) $ python -m drkcraft --help`

### View command options
`(.venv) $ python -m drkcraft CMD --help`

### Or invoke CMake and run directly:

#### Build
1. `$ cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug`
2. `$ cmake --build build --config Debug`

#### Run
* `$ build/bin/Debug/DrkCraft` (Linux)
* `$ build\bin\Debug\DrkCraft.exe` (Windows)

## Dependencies

* **C++ 20**
* **CMake 3.19**
* **Python 3.10:** Development tools, Launcher
* [**Backward:**](https://github.com/bombela/backward-cpp) Stacktrace printing
* [**Dear ImGui:**](https://github.com/ocornut/imgui) GUI rendering
* [**EnTT:**](https://github.com/skypjack/entt) Entity component system
* [**FastNoiseLite:**](https://github.com/Auburn/FastNoiseLite) Noise generation
* [**{fmt}:**](https://github.com/fmtlib/fmt) Text formatting
* [**Glad:**](https://github.com/Dav1dde/glad) OpenGL loader
* [**GLFW:**](https://github.com/glfw/glfw) OpenGL context, window, input, events
* [**GLM:**](https://github.com/g-truc/glm) Math library
* [**minimp3:**](https://github.com/lieff/minimp3) .mp3 audio file loading
* [**Magic Enum:**](https://github.com/Neargye/magic_enum): Static reflection for enums
* [**Ogg:**](https://gitlab.xiph.org/xiph/ogg) libogg audio codec
* [**OpenAL Soft:**](https://github.com/kcat/openal-soft) OpenAL audio API
* [**RapidJSON:**](https://github.com/Tencent/rapidjson) JSON parser and writer
* [**spdlog:**](https://github.com/gabime/spdlog) Logging library
* [**stb:**](https://github.com/nothings/stb)
  - **stb_image:** Image file loading
  - **stb_image_write:** Image file saving
  - **stb_rect_pack:** 2D rectangle packer
  - **stb_truetype:** TrueType fonts
* [**tinyobjloader:**](https://github.com/tinyobjloader/tinyobjloader) .obj mesh file loading
* [**Vorbis:**](https://github.com/xiph/vorbis) .ogg audio file loading
* [**yaml-cpp:**](https://github.com/jbeder/yaml-cpp) YAML parser and writer

## License
* [DrkCraft license](/LICENSE.txt)
* [Library licenses](/lib/LICENSE.toml)
* [Assets license](/assets/LICENSE.txt)
* [Launcher license](/launcher/LICENSE.txt)
