cmake_minimum_required(VERSION 3.10)
project(DonkeyKongSFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# source
file(GLOB SOURCES "source/*.cpp")

# tmxlite
set(TMXLITE_STATIC_LIB ON CACHE BOOL "Build tmxlite as static library")
add_subdirectory(external/tmxlite)

# Download SFML
include(FetchContent)

set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static library" FORCE)

FetchContent_Declare(

  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.2
)
FetchContent_MakeAvailable(SFML)

# exe
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
   PRIVATE
    tmxlite
    sfml-graphics 
)

set(SFML_DLL_DIR "${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/$<CONFIG>")

# DLL
set(SFML_DLLS
    "${SFML_DLL_DIR}/sfml-graphics-d-2.dll"
    "${SFML_DLL_DIR}/sfml-window-d-2.dll"
    "${SFML_DLL_DIR}/sfml-system-d-2.dll"
)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLLS}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
