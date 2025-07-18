cmake_minimum_required(VERSION 3.16)

project(Aerofootball)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

set(SDLIMAGE_VENDORED OFF)
# set(SDLIMAGE_AVIF OFF)	# disable formats we don't use to make the build faster and smaller.
# set(SDLIMAGE_BMP OFF)
# set(SDLIMAGE_JPEG OFF)
# set(SDLIMAGE_WEBP OFF)
# set(SDLIMAGE_TIFF OFF)
# This assumes the SDL_image source is available in vendored/SDL_image
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(Aerofootball WIN32 "main.cpp"   "globals.cpp"  "entity.cpp" "components/texture.cpp" "components/position.cpp" "components/size.cpp" "components/velocity.cpp" "draw.cpp" "systems/collision.cpp" "components/collider.cpp" "point.cpp" "components/control.cpp" "control_system.cpp" "components/debug.cpp")
target_compile_features(Aerofootball PRIVATE cxx_std_20)

file(COPY "assets/fieldScreen.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/ball.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/playerRed.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/playerBlue.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/wall.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets/center.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Link to the actual SDL3 library.
target_link_libraries(Aerofootball PRIVATE SDL3_image::SDL3_image SDL3::SDL3)
