cmake_minimum_required(VERSION 3.10)
project(DonkeyKongSFML)

# Estándar mínimo de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir headers desde carpeta include/
include_directories(${PROJECT_SOURCE_DIR}/include)

# Añadir todos los .cpp del directorio source/
file(GLOB SOURCES "source/*.cpp")

# Crear el ejecutable con los archivos fuente
add_executable(${PROJECT_NAME} ${SOURCES} "source/Player.cpp")

# Si SFML no está en variables del sistema, indicá rutas manualmente:
# (Modificá estas rutas si es necesario según donde tengas SFML)

# set(SFML_ROOT "C:\Users\Julian\SFML")

# Buscar la librería SFML
set(SFML_DIR "C:/Users/Julian/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Enlazar SFML al ejecutable
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
