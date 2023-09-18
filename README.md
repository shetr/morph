
# Morph Engine

## Build

Clone project with submodules:

```bash
git clone --recursive --depth=1 https://gitlab.com/shetr/morphengine.git
```

Or update submodules after clone:

```bash
git clone --depth=1 https://gitlab.com/shetr/morphengine.git
git submodule update --init --recursive
```

Configure cmake project:

```bash
# debug config
cmake -S . -B build/Debug -DCMAKE_BUILD_TYPE=Debug
# release config
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release
# deploy config
cmake -S . -B build/Deploy -DCMAKE_BUILD_TYPE=Release -DENABLE_DEPLOY=true
# profile debug config
cmake -S . -B build/ProfileDebug -DCMAKE_BUILD_TYPE=Debug -DENABLE_PROFILING=true
# profile release config
cmake -S . -B build/ProfileRelease -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILING=true
```

Build cmake project:

For fast build is recommended adding `-j N` to end, where N is number of threads.

```bash
# debug build all
cmake --build build/Debug
# release build all
cmake --build build/Release
# deploy build all
cmake --build build/Deploy
# profile debug build all
cmake --build build/ProfileDebug
# profile release build all
cmake --build build/ProfileRelease


# debug build specific target
cmake --build build/Debug --target your_target
# release build specific target
cmake --build build/Release --target your_target
# deploy build specific target
cmake --build build/Deploy --target your_target
# profile debug build specific target
cmake --build build/ProfileDebug --target your_target
# profile release build specific target
cmake --build build/ProfileRelease --target your_target
```
