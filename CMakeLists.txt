   cmake_minimum_required(VERSION 3.20)
   project(GameCatAndMouse)

   # Verifica se o raylib está instalado, caso contrário pede ao vcpkg para instalá-lo
   set(RAYLIB_VERSION 5.5)

   find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

   if (NOT raylib_FOUND) # If there's none, fetch and build raylib
       message(STATUS "Buscando Raylib Iniciado download pelo GITHUB...")
       include(FetchContent)
       FetchContent_Declare(
               raylib
               DOWNLOAD_EXTRACT_TIMESTAMP True
               URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
       )
       FetchContent_GetProperties(raylib)
       if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
           set(FETCHCONTENT_QUIET NO)
           FetchContent_MakeAvailable(raylib)
           set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

       endif()

   endif()


   # --- Configuração do Projeto ---
   message(STATUS "Configurando o projeto ...")
   file(GLOB_RECURSE SOURCES "src/*.c")
   add_executable(${PROJECT_NAME} ${SOURCES}
           src/scene_intro.c
           src/scene_intro.h
           src/scene_game.c
           src/scene_game.h
           src/scene_over.c
           src/scene_over.h
           src/utils.h
           src/utils.c
   )

   # --- Linkagem das Bibliotecas ---
   if (WIN32)
       message(STATUS "Linkando Bibliotecas para Windows...")
       target_link_libraries(${PROJECT_NAME}
               raylib
               opengl32
               gdi32
               user32
               winmm
       )
   elseif (UNIX)
       message(STATUS "Linkando Bibliotecas para Linux...")
       find_package(OpenGL REQUIRED)
       target_link_libraries(${PROJECT_NAME}
               raylib
               ${OPENGL_LIBRARIES}
               m
       )
   endif()


   # --- Configurações de Output ---
   if (EMSCRIPTEN)
       set(BUILD_DIR "../build_web")
   else()
       set(BUILD_DIR "../build")
   endif ()

   set_target_properties(${PROJECT_NAME} PROPERTIES
           RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
           ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}
           LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}
   )


   # Web Configurations
   if (EMSCRIPTEN)
       message(STATUS "COMPILANDO PARA WEB ASSEMBLY ")
       set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Os -Wall --shell-file build/shell.html -s ALLOW_MEMORY_GROWTH=1 -s TOTAL_MEMORY=67108864 -sUSE_GLFW=3 -sASSERTIONS=1 -sWASM=1 -sASYNCIFY -sGL_ENABLE_GET_PROC_ADDRESS=1 --preload-file ../resources")
       set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.

   else()
       # Desktop configuration
       message(STATUS "COMPILANDO PARA DESKTOP")

       # flags para o compilador
       set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra ")

       # Copiando assets para o build
       set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

       if (EXISTS ${RESOURCES_DIR})
           add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} ${BUILD_DIR}/resources
                   COMMENT "-Copiando pasta 'resources' para o diretório do executável (${BUILD_DIR})")
       else()
           message(WARNING "Diretório 'resources' não encontrado! Ignorando cópia.")
       endif()

       message(STATUS "${RESOURCES_DIR} ")
   endif ()


