$$$$$$$\            $$\        $$$$$$\                       $$$$$$\    $$
$$  __$$\           $$ |      $$  __$$\                     $$  __$$\   $$ |
$$ |  $$ | $$$$$$\  $$ |  $$\ $$ /  \__| $$$$$$\   $$$$$$\  $$ /  \__|$$$$$$
$$ |  $$ |$$  __$$\ $$ | $$  |$$ |      $$  __$$\  \____$$\ $$$$\     \_$$  _|
$$ |  $$ |$$ |  \__|$$$$$$  / $$ |      $$ |  \__| $$$$$$$ |$$  _|      $$ |
$$ |  $$ |$$ |      $$  _$$<  $$ |  $$\ $$ |      $$  __$$ |$$ |        $$ |$$\
$$$$$$$  |$$ |      $$ | \$$\ \$$$$$$  |$$ |      \$$$$$$$ |$$ |        \$$$$  |
\_______/ \__|      \__|  \__| \______/ \__|       \_______|\__|         \____/

Minecraft-based game in C++ created using an OpenGL renderer

Minecraft2??

Videos
Youtube
Manual
manual.md
Images
Game
Dependencies

C++ 20
CMake
Python 3.10
Dear ImGui
EnTT
FastNoiseLite
{fmt}
Glad
GLFW
GLM
minimp3
Magic Enum
Ogg
OpenAL Soft
RapidJSON
spdlog
stb
tinyobjloader
Vorbis
yaml-cpp

Build Instructions

$ git clone --recursive https://github.com/D3r3k23/DrkCraft DrkCraft
$ cd DrkCraft
$ scripts\setup.bat (Windows) || $ source scripts/setup.sh (Linux)
$ sudo apt install xorg-dev (Debian/Ubuntu/Linux Mint)
(.venv) $ python -m drkcraft build
(.venv) $ python -m drkcraft run
