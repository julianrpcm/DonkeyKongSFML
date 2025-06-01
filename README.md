# DonkeyKongSFML

Version SFML 2.6.2
Version cmake 3.31.2
Visual Studio 2022

IMPORTANTE
Al descargar el proyecto de GitHub, se descarga como DonkeyKongSFML-main, antes de hacer la build, renombrar la carpeta y quitar "-main", si no no cargara los tilests.

Cambiar en el CMakeLists.txt, la ruta de SFML con barras normales "/": "set(SFML_DIR "TU/RUTA")"

Despues se puede lanzar desde la consola:
mkdir build
cd build
cmake ..
cmake --build .
Introducir los DLL de tu SFML (SFML/bin) en la carpeta build/Debug del proyecto
.\Debug\DonkeyKongSFML.exe