@echo off
echo Generating project with CMake...
if not exist build mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
pause