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

There is currently only support for Windows.

If you wish to get some autocomplete and info in Visual Studio please see the VisualStudioExtension subproject. 

It will build to a VSIX file, this is Visual Studio's extension file format. To install it simply open the VSIX file and follow the installation process. 

Because of this extension it is highly recommended that you use Visual Studio for this project.

## Install

If you want to run it in Visual Studio then go to the Engine project's properties > Debugging
 - Environment > and set it to `PATH=%PATH%;$(SolutionDir)\Engine\Libraries\dll`
 - Working Directory > and set it to `$(SolutionDir)`

Now you should be able to run it inside of Visual Studio.

If you wish to run it in editor mode, go to the Engine project's properties > Debugging > Command Arguments and add `--editor` since it defaults to game mode.

## Debug build using CMake

Run `debug.bat`

The build can be found at `Build/Debug`

Simply run `editor.bat` or `game.bat` depending on what you want to use. 

## Release build using CMake

Run `release.bat`

The build can be found at `Build/Release`

Simply run `editor.bat` or `game.bat` depending on what you want to use. 