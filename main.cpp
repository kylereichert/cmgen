#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <fstream>

std::string sdl3_cmake = R"(
cmake_minimum_required(VERSION 3.30)

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

)";

void sdl3Init(std::string name = "default") {
  std::ofstream file;
  file.open("text.txt");
  if (!file.is_open()) {
    std::cerr << "Error creating file" << '\n';
  } else {
    file << sdl3_cmake;
    file.close();
    
    std::cout << "CMakeList.txt created for SDL3 project: " << name << '\n';
    std::cout << "Note, if you need another version, change the git tag!" << '\n';
    }

}

int main(int argc, char* argv[]) {
  // Standard output with too few inputs
  if (argc < 2) {
    std::cerr << "Usage: cmgen <command> [options]\n";
    return 1;
  }  

  std::vector<std::string> args(argv, argc + argv);
  
  // Command mappings
  std::unordered_map<std::string, std::function<void()>> commands {
    { "--sdl3", []() { std::cout << "yeah" << '\n'; }},
    { "Butt", []() { std::cout << "bah" << '\n'; }},
  };
  
  std::string cmd = args[1];
   
  // Test
  sdl3Init("yeah");


  // Find the argument or output error
  if (auto arg = commands.find(cmd); arg != commands.end()) {
    arg->second();
  } else {
    std::cerr << "Unkown Command: " << cmd << '\n';
    return 1;
  }

  // for (int i = 1; i < argc; i++) {
  //   std::cout << argv + i << "\n";
  // }
  


  // 
}
