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
set(SDLTTF_HARFBUZZ OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND "$ENV{MINGW_BUILD}" STREQUAL "1")
    set(SDL_VIDEO_DRIVER_X11 OFF CACHE BOOL "" FORCE)
    set(SDL_VIDEO_DRIVER_WAYLAND OFF CACHE BOOL "" FORCE)
    set(SDL_AUDIO_DRIVER_ALSA OFF CACHE BOOL "" FORCE)
    set(SDL_AUDIO_DRIVER_PIPEWIRE OFF CACHE BOOL "" FORCE)
    set(SDL_AUDIO_DRIVER_PULSEAUDIO OFF CACHE BOOL "" FORCE)
    set(SDL_AUDIO_DRIVER_JACK OFF CACHE BOOL "" FORCE)
    set(SDL_VIDEO_DRIVER_WINDOWS ON CACHE BOOL "" FORCE)
    set(SDL_AUDIO_DRIVER_WASAPI ON CACHE BOOL "" FORCE)
endif()

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


# Copy MinGW dlls
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND "$ENV{MINGW_BUILD}" STREQUAL "1")
	add_custom_command(TARGET Aerofootball POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			/usr/lib/gcc/x86_64-w64-mingw32/13-posix/libstdc++-6.dll
			/usr/lib/gcc/x86_64-w64-mingw32/13-posix/libgcc_s_seh-1.dll
			/usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll
			$<TARGET_FILE_DIR:Aerofootball>
	)
endif()

# Copy all resources
file(GLOB RESOURCE_FILES ${CMAKE_SOURCE_DIR}/assets/*.png ${CMAKE_SOURCE_DIR}/assets/*.ttf)
add_custom_command(TARGET Aerofootball POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RESOURCE_FILES}
        $<TARGET_FILE_DIR:Aerofootball>
)

# Link to the actual SDL3 library.
target_link_libraries(Aerofootball PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
