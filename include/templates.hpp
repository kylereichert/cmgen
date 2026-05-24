#pragma once

#include <string>
#include <format>


inline std::string sdl3Body(std::string name = "Project") { 

  std::string cmakebody = 
  std::format(R"(cmake_minimum_required(VERSION 3.30)

project({})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.4
)

FetchContent_MakeAvailable(SDL3)

add_executable(${{PROJECT_NAME}} main.cpp)

target_link_libraries(${{PROJECT_NAME}} PRIVATE
  SDL3::SDL3
)
)", name);
  return cmakebody;
}

inline std::string basicBody(std::string name = "Project") { 

  std::string cmakebody = 
  std::format(R"(cmake_minimum_required(VERSION 3.30)

project({})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${{PROJECT_NAME}} main.cpp)

)", name);
  return cmakebody;
}