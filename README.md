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

This is required for the usd test example. Just make sure your CMAKE_PREFIX_PATH is set to where you installed USD.


# Downloading
There are some submodules so don't forget `--recursive` or `git submodule update --init` after the fact

# Build
I tend to use VSCode and the terminal for build/test cycle:
```sh
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=<llvm_install_path>;<usd_install_path>`
cmake --build build -j12 --config Release
```

I then debug in VS when C++ does C++ stuff. The `RelWithDebInfo` config is set up for a debugging experience that doesn't make you want to jump out the window:
`cmake -B buildvs`

then open `buildvs/babble.sln`` in VS, switch to `RelWithDebInfo`` and update your debug working directory and arguments as per the testing section below

# Testing
`./build/bbl/bbl-translate test/test001.cpp -- -std=c++17 -Ibbl/include`
`./build/bbl/bbl-translate test/test002.cpp -- -std=c++17 -Ibbl/include`
...
`./build/bbl/bbl-translate test/test020.cpp -- -std=c++17 -Ibbl/include`

No automated testing in place yet. Just inspecting the output from the small test files above. Need to add output capturing and textual comparison to expected results.

The test USD binding should be built automaticaly if you set your CMAKE_PREFIX_PATH correctly to your USD installation. To run it:
```sh
> .\build\test\bbl-usd\usd-c-test01.exe .\test\bbl-usd\test01.usda
Warning: in _ReportErrors at line 2971 of C:\sbf\rez-cook\usd\23.08\pxr\usd\usd\stage.cpp -- In </World/redtorus>: Could not open asset @c:/Users/alanglands/code/babble2/test/bbl-usd/redtorus.usda@ for payload introduced by @test/bbl-usd/test01.usda@</World/redtorus>. (instantiating stage on stage @test/bbl-usd/test01.usda@ <000002300ED57B00>)
/
World
Sphere
Cube
Looks
PreviewSurface
Shader
redtorus
Camera
Environment
Sky
DistantLight
Looks
Grid
Shader
ground
groundCollider
```