# OpenGL Game Engine

## Info

This engine makes use of a set of libraries:
- Assimp
- Glad
- GLFW
- GLM
- ImGui
- Json
- Khr
- Magic Enum
- Mono
- Stb

## Support

There is currently only support for Windows

## Install

If you want to run it in Visual Studio then go to the Engine project's properties > Debugging > Environment and set it to `PATH=%PATH%;$(SolutionDir)\Engine\Libraries\dll`

Now you should be able to run it inside of Visual Studio. If you wish to run it in editor mode add the argument in the environment `--editor` since it defaults to game mode.

## Debug build using CMake

The build can be found at Build/Debug
Simply run editor.bat or game.bat depending on what you want to use. 

## Release build using CMake

The build can be found at Build/Release
Simply run editor.bat or game.bat depending on what you want to use. 