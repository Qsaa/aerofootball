#include "globals.hpp"
#include "entity.hpp"
#include "components/texture.hpp"
#include "components/position.hpp"
#include "components/size.hpp"

void draw(Entities& entities)
{
    for (auto& entity : entities)
    {
        auto tex = entity.getComponent<Texture>();
        auto pos = entity.getComponent<Position>();
        auto size = entity.getComponent<Size>();
        auto rect = SDL_FRect{ pos->x_, pos->y_, size->w_, size->h_ };
        SDL_RenderTexture(renderer, tex->texture_, nullptr, &rect);
    }
}