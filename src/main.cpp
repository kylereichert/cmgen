#include <expected>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include "templates.hpp" // Template location


std::expected<void, std::string> init(std::function<std::string(std::string)> body, std::string name = "Project") {
  std::string filename = "CMakeList.txt"; // Change this to CMakeList.txt after testing

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
  // Error if too few arguments are supplied.
  if (argc < 2) {
    std::cerr << "Usage: cmgen <command> [options]\n";
    return 1;
  }  
  // Just convert the args to match modern C++
  std::vector<std::string> args(argv, argc + argv);
 
  // Valid flags for CMakeList.txt types
  std::unordered_map<std::string, std::function<void(std::string)>> commands {
    { "--sdl3", [](std::string name) { handleResult(init(sdl3Body, name)); }},
    { "--basic", [](std::string name) { handleResult(init(basicBody, name)); }},
  };

  // Main parsing logic
  std::string cmd = args[1];
  std::string name = (argc > 2) ? argv[2] : "Project";
  if (auto it = commands.find(cmd); it != commands.end()) {
    it->second(name);
  } else {
    std::cerr << "Unknown Command: " << cmd << '\n';
  }
}
