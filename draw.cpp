#include "globals.hpp"
#include "entity.hpp"
#include "components/texture.hpp"
#include "components/position.hpp"
#include "components/size.hpp"

void draw(Entities& entities)
{
    // for (auto& entity : entities)
    // {
    //     if (!entity.hasComponent<Texture, Position, Size>())
    //     {
    //         continue;
    //     }
    //     auto tex = entity.getComponent<Texture>();
    //     auto pos = entity.getComponent<Position>();
    //     auto size = entity.getComponent<Size>();
    //     auto rect = SDL_FRect{ pos->x_, pos->y_, size->w_, size->h_ };
    //     SDL_RenderTexture(renderer, tex->texture_, nullptr, &rect);      
    // }
    for(size_t i = 0; i < entities.size(); ++i)
    {
        if(!entities[i].hasComponent<Texture, Position, Size>())
        {
            continue;
        }
        auto tex = entities[i].getComponent<Texture>();
        auto pos = entities[i].getComponent<Position>();
        auto size = entities[i].getComponent<Size>();
        auto rect = SDL_FRect{ pos->x_, pos->y_, size->w_, size->h_ };
        SDL_RenderTexture(renderer, tex->texture_, nullptr, &rect); 
    }
}