include_guard()

include(FetchContent)

FetchContent_Declare(
	SDL_ttf
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
	GIT_TAG release-3.2.2
)
FetchContent_MakeAvailable(SDL_ttf)
