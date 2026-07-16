cmake_minimum_required(VERSION 3.16)

project(Aerofootball)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_LOG_DOWNLOADS ON)

set(SDLIMAGE_VENDORED OFF)
set(SDLTTF_VENDORED ON)

include(cmake/SDL.cmake)
include(cmake/SDL_image.cmake)
include(cmake/SDL_ttf.cmake)


# Create your game executable target as usual
add_executable(Aerofootball WIN32
	"main.cpp"
	"globals.cpp"
	"entity.cpp"
	"components/texture.cpp"
	"components/position.cpp"
	"components/size.cpp"
	"components/velocity.cpp"
	"draw.cpp"
	"systems/collision.cpp"
	"components/collider.cpp"
	"point.cpp"
	"components/control.cpp"
	"control_system.cpp"
	"components/debug.cpp"
	"components/goal.cpp"
)
target_compile_features(Aerofootball PRIVATE cxx_std_20)

file(COPY "assets/fieldScreen.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/ball.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/playerRed.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/playerBlue.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/wall.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/center.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/uefa-nations-titling.ttf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Link to the actual SDL3 library.
target_link_libraries(Aerofootball PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
