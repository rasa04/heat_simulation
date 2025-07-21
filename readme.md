Requirements:

CMake (в PATH) → cmake.org/download → Windows x64 Installer (cmake-*-windows-x86_64.msi)
Visual Studio 2022 Build Tools → "Desktop Development with C++"
CUDA Toolkit 12.x
Git

step 1:
cd libs
git clone https://github.com/ocornut/imgui.git

step 2:
// From project dir
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install glfw3 glew

step 3:
// Execute build.bat script
./build.bat

step 4:
// Run executable:
.\build\Release\HeatSim.exe
