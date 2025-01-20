cmake_minimum_required(VERSION 3.14)
project(GameCatAndMouse)

# --- Diretórios ---
set(SOURCE_DIR src)
set(BUILD_DIR build)

# --- Arquivos Fonte ---
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")

# --- Tenta encontrar uma raylib instalada localmente ---
find_package(raylib 5.5)

# --- Baixa a Raylib se não for encontrada ---
include(FetchContent)
if (NOT raylib_FOUND)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
            DOWNLOAD_EXTRACT_TIMESTAMP True
    )
    FetchContent_MakeAvailable(raylib)
endif()

# --- Desativa a construção dos exemplos da Raylib ---
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# --- Executável ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Linkagem de Bibliotecas ---
if (WIN32)
    message(STATUS "Configurando para Windows...")
    target_link_libraries(${PROJECT_NAME}
            raylib
            opengl32
            gdi32
            user32
            winmm
    )
elseif (UNIX)
    message(STATUS "Configurando para Linux...")
    find_package(OpenGL REQUIRED) # Busca o OpenGL no Linux ou UNIX
    target_link_libraries(${PROJECT_NAME}
            raylib
            ${OPENGL_LIBRARIES}
            m # Para matemáticas, em sistemas Linux
    )
endif()

# --- Diretório de Inclusão ---
target_include_directories(${PROJECT_NAME} PUBLIC "${raylib_SOURCE_DIR}/src")

# --- Diretório de Saída para o Executável ---
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Diretório de Saída para os Arquivos Objeto ---
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BUILD_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BUILD_DIR})

# Diretório de origem das pastas
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(SOUNDS_DIR "${CMAKE_SOURCE_DIR}/sounds")

# Diretório de destino - onde o executável será gerado
set(OUTPUT_DIR "${BUILD_DIR}")

# Certifique-se de que a variável esteja configurada
if (NOT OUTPUT_DIR)
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}") # Fallback padrão
endif()

# Copiar as pastas para o mesmo diretório que o executável após a build
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} ${OUTPUT_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOUNDS_DIR} ${OUTPUT_DIR}/sounds
        COMMENT "Copiando pastas 'resources' e 'sounds' para o diretório do executável (${OUTPUT_DIR})"
)