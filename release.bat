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

echo [INFO] Release build completed successfully.

endlocal