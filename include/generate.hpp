#pragma once
#include <expected>
#include <filesystem>
#include <iostream>
#include <functional>
#include <string>
#include <fstream>


using BodyFnArg = std::function<std::string(std::string)>;
using BodyFn = std::function<std::string()>;
using Result = std::expected<void, std::string>;

struct ProjectType {
  BodyFnArg cmakeBody;
  BodyFn mainBody;
};

// Not an elegent function wrapper, but makes it easier to read.
inline void handleResult(std::expected<void, std::string> result) {
  // Handles the results for the main CLI arguments
  if (!result) {
    std::cerr << "Init Failed: " << result.error() << "\n";
  }
}

inline Result writeMain(std::ofstream& file, BodyFn body) {
  if (!file.is_open()) {
    return std::unexpected("main.cpp failed to open!");
  }

  file << body();

  return {};
}

inline Result writeCMake(std::ofstream& file, BodyFnArg body, std::string projectname) {
  if (!file.is_open()) {
    return std::unexpected("CMakeLists.txt failed to open!");
  }

  file << body(projectname);

  return {};
}

inline Result init(const ProjectType& project, std::string name = "Project") {

  std::string cmakename = "CMakeLists.txt";
  std::string mainname = "main.cpp";

  if (std::filesystem::exists(cmakename)) {
    return std::unexpected("CMakeList.txt already exists!");
  }
  if (std::filesystem::exists(mainname)){
    return std::unexpected("main.cpp already exists!");
  };

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
