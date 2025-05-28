cmake_minimum_required(VERSION 3.10)
project(DonkeyKongSFML)

# Estándar mínimo de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir headers desde carpeta include/
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "source/*.cpp")

set(TMXLITE_STATIC_LIB ON CACHE BOOL "Build tmxlite as static library")

add_subdirectory(external/tmxlite)
add_executable(${PROJECT_NAME} ${SOURCES} ${TMXLITE_SRC} ${PUGIXML_SRC})
# Crear el ejecutable con los archivos fuente


# Si SFML no está en variables del sistema, indicá rutas manualmente:
# (Modificá estas rutas si es necesario según donde tengas SFML)

# set(SFML_ROOT "C:\Users\Julian\SFML")

# Buscar la librería SFML
set(SFML_DIR "C:/Users/Julian/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Enlazar SFML al ejecutable
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tmxlite
    sfml-graphics sfml-window sfml-system
)

