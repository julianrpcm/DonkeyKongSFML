cmake_minimum_required(VERSION 3.10)
project(DonkeyKongSFML)

# Estándar mínimo de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir headers desde carpeta include/
include_directories(${PROJECT_SOURCE_DIR}/include)

# Archivos fuente principales
file(GLOB SOURCES "source/*.cpp")

# Incluir tmxlite como submódulo
set(TMXLITE_STATIC_LIB ON CACHE BOOL "Build tmxlite as static library")
add_subdirectory(external/tmxlite)

# Si usas pugixml también como fuente externa:
# set(PUGIXML_SRC "") # define si lo usas directamente

# Buscar SFML instalado
set(SFML_DIR "C:/Users/Julian/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Crear el ejecutable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${TMXLITE_SRC}  # opcional si tmxlite necesita fuentes explícitas
    ${PUGIXML_SRC}  # opcional si pugixml necesita fuentes explícitas
)

# Enlazar bibliotecas necesarias
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tmxlite
    sfml-graphics sfml-window sfml-system
)

# Copiar assets automáticamente al directorio de build (opcional pero recomendable)
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
