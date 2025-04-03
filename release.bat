@echo off
setlocal

echo [INFO] Creating build folder (Release)...
if not exist build (
    mkdir build
)

cd build

echo [INFO] Running CMake configure (Release mode)...
cmake -DCMAKE_BUILD_TYPE=Release ..

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %errorlevel%
)

echo [INFO] Building project (Release)...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    exit /b %errorlevel%
)

echo [INFO] Copying final build to /release folder...

cd ..

REM Optional: clean or recreate release folder
if exist release (
    rmdir /s /q release
)
mkdir release

REM Copy built EXE
copy build\Release\opengl-game-engine.exe release\

REM Copy Assets folder
xcopy Assets release\Assets /E /I /Y >nul

REM Copy DLLs
copy Libraries\dll\assimp-vc143-mt.dll release\ >nul

echo [INFO] Release build prepared in /release

endlocal
