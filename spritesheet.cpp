#include "spritesheet.h"

void SpriteSheet::scale_sprite(sf::Sprite *sprite, int scale) {
    if ((!scale && default_scale) || scale) {
        if (!scale) {
            scale = default_scale;
        }
        sprite->scale(scale, scale);
    }
}

SpriteSheet::SpriteSheet(std::string name, int scale) {
    sheet.loadFromFile(name);
    if (scale <= 1) {
        default_scale = 0;
    }
    else {
        default_scale = scale;
    }
}

sf::Sprite SpriteSheet::get_sprite(sf::Vector2u dimensions, int x, int scale) {
    sf::Sprite sprite;
    sprite.setTexture(sheet);
    sprite.setTextureRect(sf::IntRect(x, farthest_y, dimensions.x, dimensions.y));
    sprite.setOrigin(dimensions.x / 2, dimensions.y / 2);
    scale_sprite(&sprite, scale);
    return sprite;
}

SpriteVector SpriteSheet::get_sprites(sf::Vector2u dimensions, int x, int scale, bool update) {
    SpriteVector sprites = {get_sprite(dimensions, x, scale)};
    if (update) {
        farthest_y += dimensions.y;
    }
    return sprites;
}

SpriteVector SpriteSheet::get_sprites(std::vector<sf::Vector2u> dimensions, int x, int scale, bool update) {
    SpriteVector sprites;
    int max_y = 0;
    for (sf::Vector2u i_dimensions : dimensions) {
        sprites.push_back(get_sprite(i_dimensions, x, scale));
        x += i_dimensions.x;
        if (i_dimensions.y > max_y) {
            max_y = i_dimensions.y;
        }
    }
    if (update) {
        farthest_y += max_y;
    }
    return sprites;
}

SpriteVector SpriteSheet::get_custom_sprites(std::vector<sf::Vector2u> dimensions, int constant, int index, int x, int scale, bool update) {
    SpriteVector sprites;
    int max_y = 0;
    for (sf::Vector2u i_dimensions : dimensions) {
        sf::Vector2u current_dimensions;
        if (index == 0) {
            current_dimensions.x = constant;
            current_dimensions.y = i_dimensions.y;
        } else {
            current_dimensions.y = constant;
            current_dimensions.x = i_dimensions.x;
        }
        sprites.push_back(get_sprite(current_dimensions, x, scale));
        x += i_dimensions.x;
        if (i_dimensions.y > max_y) {
            max_y = i_dimensions.y;
        }
    }
    if (update) {
        farthest_y += max_y;
    }
    return sprites;
}

SpriteVector SpriteSheet::get_custom_sprites(sf::Vector2u dimensions, int number, int x, int scale, bool update) {
    SpriteVector sprites;
    int max_y = 0;
    for (int i = 0; i < number; i++) {
        sprites.push_back(get_sprite(dimensions, x, scale));
        x += dimensions.x;
    }
    if (update) {
        farthest_y += dimensions.y;
    }
    return sprites;
}
