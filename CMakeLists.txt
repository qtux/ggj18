# project meta data
project(Skillswitch)
set(main_target "run")

# minimum required cmake version
cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE tmxlite tmxlite/tmxlite/src/*.cpp tmxlite/tmxlite/src/miniz.c)
add_executable(${main_target} ${sources} ${tmxlite})

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

# find SFML
# Ubuntu-specific fix:
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} /usr/share/SFML/cmake/Modules/)
find_package(SFML 2.3 REQUIRED audio graphics window system)

# find Box2D
find_package(Box2D)

# include SFML and tmxlite
target_include_directories(${main_target} PRIVATE
	${SFML_INCLUDE_DIR}
	${BOX2D_INCLUDE_DIR}
	tmxlite/tmxlite/include tmxlite/tmxlite/include/tmxlite/detail
)

# link executable to SFML
target_link_libraries(${main_target} ${SFML_LIBRARIES} ${BOX2D_LIBRARIES})



