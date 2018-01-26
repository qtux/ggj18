# project meta data
project(Skillswitch)
set(main_target "run")

# minimum required cmake version
cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(${main_target} ${sources})

# force C++14 standard and disable custom standard library extensions
set_target_properties(${main_target} PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

# add extra warnings to the main target
target_compile_options(${main_target} PRIVATE
	-Wall
	-pedantic
	-Wextra
)

# find and link SFML
find_package(SFML 2.3 REQUIRED audio graphics window system)
target_link_libraries(${main_target} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_include_directories(${main_target} PRIVATE ${SFML_INCLUDE_DIR})
