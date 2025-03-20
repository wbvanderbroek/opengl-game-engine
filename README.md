# How to build

## First time

### Linux (and mac?)

```bash
git clone https://github.com/glfw/glfw.git
cd glfw
cmake -S . -B build -DBUILD_SHARED_LIBS=ON
cmake --build build
sudo cmake --install build
cd ..
mkdir build && cd build && cmake .. && cmake --build .
```

except that it somehow broke during https://github.com/wbvanderbroek/opengl-game-engine/commit/ebc372265fc46729faacc27881d1630b575a80cd

### Windows

```bash
mkdir build && cd build && cmake .. && cmake --build .
```

## Normal build

```bash
cd build && cmake .. && cmake --build .
```

The build can be found and run in `/build/Debug/` when on Windows.
On Linux it can simply be found at `/build/`

# Running the build

This should be as simple as going into `/build/Debug/` or `/build/` and running the executable since all neccessary files should be copied over.

If you are running it in Visual Studio you might have to copy the assimp dll from the libraries folder into the debug/release folder.