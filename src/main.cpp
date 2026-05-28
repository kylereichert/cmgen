#include <unordered_map>
#include <vector>
#include "generate.hpp"
#include <templates.hpp>


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: cmgen <command> [options]\n";
    return 1;
  }  
  
  // Just convert the args to match modern C++
  std::vector<std::string> args(argv, argc + argv);

  // New mapping, should pass a struct
  std::unordered_map<std::string, ProjectType> projects {
    {"--basic", {basicCMake, basicMain} },
    {"--sdl3", {sdl3CMake, sdl3Main} },
  };

  // Main parsing logic
  std::string cmd = args[1];
  std::string name = (argc > 2) ? argv[2] : "Project";

  if (auto it = projects.find(cmd); it != projects.end()) {
    handleResult(init(it->second, name));
  } else {
    std::cerr << "Usage: cmgen <command> [options]\n";
  }

  return 0;
}
