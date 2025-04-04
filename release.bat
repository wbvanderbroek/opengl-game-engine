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

echo [INFO] Copying final build to build/release folder...

REM Optional: clean or recreate release folder inside build
if exist Release (
    rmdir /s /q Release
)
mkdir Release

REM Copy built EXE
copy Release\opengl-game-engine.exe Release\

REM Copy Assets folder
xcopy ..\Assets Release\Assets /E /I /Y >nul

REM Copy DLLs
copy ..\Libraries\dll\assimp-vc143-mt.dll Release\ >nul

echo [INFO] Release build prepared in build/Release

endlocal
