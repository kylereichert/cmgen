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

  Rule of thumb: use private for binaries, public for libraries
  target_link_libraries(${{PROJECT_NAME}} PRIVATE
    SDL3::SDL3
  )

  )", name);
  return cmakebody;
}

std::expected<void, std::string> sdl3Init(std::string name = "Project") {
  std::string filename = "test.txt";

  if (std::filesystem::exists(filename)) {
    return std::unexpected("CMakeList.txt already exists!");
  } else {
    std::ofstream file;
    file.open(filename);

    if (!file.is_open()) {
      return std::unexpected("File failed to open!");
    } 
    else {
      file << sdl3Body(name);
    }

  }
  
  std::cout << "CMakeList.txt created for SDL3 project: " << name << '\n';

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
 
 /*
  Not sure if this is the best approach. For each primary command, the command 
  should be an expected type handled byt the handleResult function. Then this 
  itself is called in a lambda function. The iterator seems to like this format.
 */ 

  std::unordered_map<std::string, std::function<void()>> commands {
    { "--sdl3", []() { handleResult(sdl3Init()); }},
  };
  
  std::string cmd = args[1];
   
  // Find the argument or output error
  if (auto it = commands.find(cmd); it != commands.end()) {
    it->second();
  } else {
    std::cerr << "Unkown Command: " << cmd << '\n';
  }

  // for (int i = 1; i < argc; i++) {
  //   std::cout << argv + i << "\n";
  // }
  


  // 
}
