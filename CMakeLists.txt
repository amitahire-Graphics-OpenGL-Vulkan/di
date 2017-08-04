##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project               (nano_engine VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)

##################################################    Options     ##################################################
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
set(PROJECT_SOURCES
  CMakeLists.txt
  cmake/assign_source_group.cmake
  cmake/import_library.cmake
  
  include/nano_engine/systems/display.hpp
  include/nano_engine/systems/input.hpp
  include/nano_engine/systems/renderer.hpp
  include/nano_engine/utility/frame_timer.hpp
  include/nano_engine/utility/gl_context.hpp
  include/nano_engine/engine.hpp
  include/nano_engine/system.hpp
)
include(assign_source_group)
assign_source_group(${PROJECT_SOURCES})

##################################################  Dependencies  ##################################################
include(import_library)

add_subdirectory(external/gl)
set_property    (TARGET gl PROPERTY FOLDER "External") 
list            (APPEND PROJECT_LIBRARIES gl)

find_package    (Boost REQUIRED)
import_library  (Boost_INCLUDE_DIRS)

find_package    (GLEW REQUIRED)
import_library  (GLEW_INCLUDE_DIRS GLEW_LIBRARIES)

find_package    (SDL2 REQUIRED)
import_library  (SDL2_INCLUDE_DIR SDL2_LIBRARY)
if   (WIN32)
  list(APPEND PROJECT_LIBRARIES winmm.lib Imm32.lib)
endif()

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} INTERFACE ${PROJECT_LIBRARIES})

# Hack for header-only project to appear in the IDEs.
add_library(${PROJECT_NAME}_ STATIC ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}_ 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME}_ PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME}_ PUBLIC ${PROJECT_LIBRARIES})
set_target_properties     (${PROJECT_NAME}_ PROPERTIES LINKER_LANGUAGE CXX)

##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing()

  set(PROJECT_TEST_SOURCES
    tests/engine_test.cpp
  )

  foreach(_SOURCE ${PROJECT_TEST_SOURCES})
    get_filename_component(_NAME ${_SOURCE} NAME_WE)
    set                   (_SOURCES tests/catch.hpp tests/main.cpp ${_SOURCE})
    add_executable        (${_NAME} ${_SOURCES})
    target_link_libraries (${_NAME} ${PROJECT_NAME})
    add_test              (${_NAME} ${_NAME})
    set_property          (TARGET ${_NAME} PROPERTY FOLDER "Tests")
    source_group          ("source" FILES ${_SOURCES})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}-config")
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  "${PROJECT_NAME}-config" DESTINATION "cmake")
export (TARGETS "${PROJECT_NAME}"        FILE        "${PROJECT_NAME}-config.cmake")