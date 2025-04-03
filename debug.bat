@echo off
setlocal

echo [INFO] Creating build folder (Debug)...
if not exist build (
    mkdir build
)

cd build

echo [INFO] Running CMake configure (Debug mode)...
cmake -DCMAKE_BUILD_TYPE=Debug ..

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %errorlevel%
)

echo [INFO] Building project (Debug)...
cmake --build . --config Debug

if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    exit /b %errorlevel%
)

echo [INFO] Debug build completed successfully.
endlocal
