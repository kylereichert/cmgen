#include <expected>
#include <filesystem>
#include <format>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <fstream>

std::string sdl3Body(std::string name = "Project") { 

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

std::expected<void, std::string> init(std::function<std::string(std::string)> body, std::string name = "Project") {
  std::string filename = "test.txt"; // Change this to CMakeList.txt after testing

  if (std::filesystem::exists(filename)) {
    return std::unexpected("CMakeList.txt already exists!");
  }

  std::ofstream file(filename);

  if (!file.is_open()) {
    return std::unexpected("File failed to open!");
  }

  file << body(name);

  return {};
}

void handleResult(std::expected<void, std::string> result) {
  // Handles the results for the main CLI arguments
  if (!result) {
    std::cerr << "Init Failed: " << result.error() << "\n";
  }
}

int main(int argc, char* argv[]) {
  // Standard output with too few inputs
  if (argc < 2) {
    std::cerr << "Usage: cmgen <command> [options]\n";
    return 1;
  }  

  std::vector<std::string> args(argv, argc + argv);
 
  // Valid flags for CMakeList.txt types
  std::unordered_map<std::string, std::function<void(std::string)>> commands {
    { "--sdl3", [](std::string name) { handleResult(init(sdl3Body, name)); }},
  };

  std::string cmd = args[1];
  std::string name = (argc > 2) ? argv[2] : "Project";
  if (auto it = commands.find(cmd); it != commands.end()) {
    it->second(name);
  } else {
    std::cerr << "Unknown Command: " << cmd << '\n';
  }
}
