# How to build

## First time

If you are not on windows do this first
```bash
git clone https://github.com/glfw/glfw.git
cd glfw
cmake -S . -B build -DBUILD_SHARED_LIBS=ON
cmake --build build
sudo cmake --install build
```

```bash
mkdir build && cd build && cmake .. && cmake --build .
```

## Normal build

```bash
cd build && cmake .. && cmake --build .
```

The build can be found and run in `/build/Debug/`

# Running the build

This should be as simple as going into `/build/Debug/` and running the .exe since all neccessary files should be copied over.