#include <expected>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include "templates.hpp" // Template location


/*
Should change the init function to possible take on just a type, such as sdl3, 
then use that to divvy up assembling of the project. Maybe another unordered list
that will have a lambda function that you can then use to open files and insert text?
*/


using BodyFnArg = std::function<std::string(std::string)>;
using BodyFn = std::function<std::string()>;
using Result = std::expected<void, std::string>;

struct ProjectType {
  BodyFnArg cmakeBody;
  BodyFn mainBody;
};

Result writeMain(std::ofstream& file, BodyFn body) {
  if (!file.is_open()) {
    return std::unexpected("main.cpp failed to open!");
  }

  file << body();

  return {};
}

Result writeCMake(std::ofstream& file, BodyFnArg body, std::string projectname) {
  if (!file.is_open()) {
    return std::unexpected("CMakeLists.txt failed to open!");
  }

  file << body(projectname);

  return {};
}

Result init(const ProjectType& project, std::string name = "Project") {

  std::string cmakename = "CMakeLists.txt";
  std::string mainname = "main.cpp";

  if (std::filesystem::exists(cmakename)) {
    return std::unexpected("CMakeList.txt already exists!");
  }
  if (std::filesystem::exists(mainname)){
    return std::unexpected("main.cpp already exists!");
  };

  // Might need to call specific write functions for this part. Once we check 
  // if the file is exists, we can open and pass them to write functions
  std::ofstream cmakefile(cmakename);
  std::ofstream mainfile(mainname);

  if (!writeCMake(cmakefile, project.cmakeBody, name)) {
    std::cerr << "Failure to create CMakeList.txt!" << '\n';
  };
  if (!writeMain(mainfile, project.mainBody)) {
    std::cerr << "Failure to create main.cpp!" << '\n';
  }

  // Probably should create src and inlude directories and wrap them up into
  // their own function.
  std::filesystem::create_directories("build");

  return {};
}

// Not an elegent function wrapper, but makes it easier to read.
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

  // New mapping, should pass a struct
  std::unordered_map<std::string, ProjectType> projects {
    {"--basic", {basicCMake, basicMain} },
    {"--sdl3", {sdl3CMake, basicMain} }, // Change main.cpp to a sdl3 template
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
