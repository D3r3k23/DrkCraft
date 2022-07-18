## Repository
```
DrkCraft
├─── .venv: Python environment
├─── assets: Game assets
├─── build: DrkCraft build directory
├─── data: Game data
│    └─── blocks.yaml: DrkCraft block information
├─── doc: Game instructions
├─── launcher: DrkCraft Launcher Python app
│    ├─── .venv: Python environment
│    ├─── build: Launcher build directory
│    ├─── scripts: Build scripts
│    └─── src
├─── lib: Third party libraries
├─── packages: DrkCraft packages
├─── python: Python tools
│    └─── drkcraft
│         ├─── build: DrkCraft build scripts
│         │    ├─── config.py
│         │    ├─── build.py
│         │    ├─── run.py
│         │    └─── package.py
│         ├─── tools: Various scripts
│         │    ├─── clean_logs.py
│         │    ├─── gen_block_texture_atlas.py
│         │    └─── profile.py
│         └─── main.py
├─── scripts: Environment setup and misc. shell scripts
├─── src
│    └─── DrkCraft
│         ├─── Application
│         │    └─── Layers
│         ├─── Audio
│         ├─── Core
│         │    ├─── Debug
│         │    └─── Type
│         ├─── Disk
│         ├─── Game
│         │    ├─── Entity
│         │    ├─── Items
│         │    ├─── Layers
│         │    ├─── Save
│         │    ├─── Systems
│         │    ├─── Util
│         │    └─── World
│         ├─── Graphics
│         │    └─── Renderer
│         ├─── Lib
│         ├─── System
│         ├─── Util
│         └─── Main.cpp
├─── tools: Misc. tools
│    └─── std_usage: Python library to analyze C++ stdlib usage
├─── vscode: .vscode directory templates
└─── CMakeLists.txt
```

## Game
```
DrkCraft
├─── assets: Game assets
├─── config: Game settings
│    ├─── config.yaml
│    ├─── settings.yaml
│    └─── keybinds.yaml
└─── data: Game data
     ├─── logs
     ├─── profile: Instrumentation profiling results
     └─── saves: DrkCraft saves
          ├─── World1
          └─── World2
```

## Save
```
World
├─── chunks
│    ├─── x.y.z.chunk
│    └─── x.y.z.chunk
├─── entities.json
├─── player.json
├─── save.json
├─── stats.json
└─── world.json
```

## Game Package
```
DrkCraft
├─── about: Package information
├─── assets: Game assets
├─── data: Game data
├─── doc: Game instructions
├─── tools: DrkCraft tools
└─── DrkCraft.exe
```

## DrkCraft Installation
```
DrkCraft
├─── Installations
│    ├─── DrkCraft-v1.0
│    ├─── DrkCraft-v1.1
│    └─── DrkCraft-v1.2
├─── Launcher
│    ├─── logs
│    ├─── resources
│    └─── drkcraft-launcher.exe
├─── Screenshots
└─── DrkCraft Launcher
```

## Launcher Package
```
drkcraft-launcher
├─── Launcher
│    ├─── resources
│    ├─── DrkCraft-v1.1
│    └─── drkcraft-launcher.exe
└─── drkcraft-installer.exe
```
