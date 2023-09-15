# Prerequisites
## clang 16
Other versions may work but are not tested.

```sh
git clone https://github.com/llvm/llvm-project.git -b llvmorg-16.0.6
cd llvm-project
cmake -S llvm -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
  -DCMAKE_INSTALL_PREFIX=<llvm_install_path>
cmake --build build --target install --config Release
# ... go make a coffee
```

## OpenUSD 23.08
Other versions should work, but may have some differences in API that will cause the binding compilation to error.


# Downloading
There are some submodules so don't forget `--recursive` or `git submodule update --init` after the fact

# Build
I tend to use VSCode and the terminal for build/test cycle:
```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=<llvm_install_path>`
cmake --build build -j12 --config Release
```

I then debug in VS when C++ does C++ stuff. The `RelWithDebInfo` config is set up for a debugging experience that doesn't make you want to jump out the window:
`cmake -B buildvs`

then open `buildvs/babble.sln`` in VS, switch to `RelWithDebInfo`` and update your debug working directory and arguments as per the testing section below

# Testing
`build/babble test/test001.cpp -- -Ibbl/include`
`build/babble test/test002.cpp -- -Ibbl/include`
...
`build/babble test/test006.cpp test/test007.cpp -- -Ibbl/include`

No automated testing in place yet. Just inspecting the output from the small test files above. 

Testing the USD binding means passing all your include paths to clang (will set this up automatically with CMake later), as well as a couple of options to shut some warnings up. On my system that looks like:
```sh
./build/bbl-dump.exe usd001.cpp -- \
  -Ibbl/include \
  -I"C:\Users\alanglands\packages\usd\23.08\935f9ef8940f2218e31d6c056bc032a600dbd374\include\" \ -I"C:\Users\alanglands\packages\boost\1.79.0\platform-windows\arch-AMD64\vs-2022\python-3.9.12\cfg-release\include\" \
  -I"C:\Users\alanglands\packages\tbb\2020.3\platform-windows\arch-AMD64\include\" \
  -I"C:\Users\alanglands\packages\python\3.9.12\platform-windows\arch-AMD64\include" \
  -Wno-deprecated-builtins -DNOMINMAX -D_MT -DBOOST_ALL_NO_LIB
```