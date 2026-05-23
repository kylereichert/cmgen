cmake_minimum_required(VERSION 3.30)

project(cmgen)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

######### Fetch libraries from github ##############
# include(FetchContent)

# FetchContent_Declare(
#   SDL3
#   GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#   GIT_TAG release-3.4.4
# )

# FetchContent_MakeAvailable(SDL3)

#####################################################

add_executable(${PROJECT_NAME} main.cpp)

# Rule of thumb: use private for binaries, public for libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  # SDL3::SDL3
)


