include_guard()

include(FetchContent)

FetchContent_Declare(
	SDL
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.4.12
)
FetchContent_MakeAvailable(SDL)
