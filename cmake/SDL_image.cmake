include_guard()

include(FetchContent)

FetchContent_Declare(
	SDL_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG release-3.4.4
)
FetchContent_MakeAvailable(SDL_image)
