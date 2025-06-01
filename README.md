# DonkeyKongSFML

SFML version 2.6.2
Version cmake 3.31.2
Visual Studio 2022

IMPORTANT
When downloading the project from GitHub, it is downloaded as DonkeyKongSFML-main, before making the build, rename the folder and remove ‘-main’, otherwise it will not load the tilests.

Change in the CMakeLists.txt, the SFML path with normal slashes ‘/’: ‘set(SFML_DIR “ YOUR/ROUTE”)’.

Then you can launch it from the console:
mkdir build
cd build
cmake ...
cmake --build .
Insert the DLLs of your SFML (SFML/bin) in the folder build/Debug of the project
.\Debug\DonkeyKongSFML.exe