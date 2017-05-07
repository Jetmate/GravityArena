#ifndef GRAVITYARENA_SPRITESHEET_H
#define GRAVITYARENA_SPRITESHEET_H

#include <SFML/Graphics.hpp>

typedef std::vector<sf::Sprite> SpriteVector;

class SpriteSheet {
private:
    sf::Texture sheet;
    int default_scale;
    int farthest_y = 0;

    void scale_sprite(sf::Sprite * sprite, int scale);

public:
    SpriteSheet(std::string p_name, int p_scale);
    sf::Sprite get_sprite(sf::Vector2u dimensions, int x=0, int scale=0);
    SpriteVector get_sprites(sf::Vector2u dimensions, int x=0, int scale=0, bool update=true);
    SpriteVector get_sprites(std::vector<sf::Vector2u> dimensions, int x=0, int scale=0, bool update=true);
    SpriteVector get_custom_sprites(std::vector<sf::Vector2u> dimensions, int constant, int index, int x=0, int scale=0, bool update=true);
    SpriteVector get_custom_sprites(sf::Vector2u dimensions, int number, int x=0, int scale=0, bool update=true);
};

#endif //GRAVITYARENA_SPRITESHEET_H
