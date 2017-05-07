#ifndef GRAVITYARENA_GAME_H
#define GRAVITYARENA_GAME_H

//#include <SFML/Graphics.hpp>

#include <iostream>

namespace PlayerSpriteTypes {
    enum Enum {
        IDLE,
        ACCELERATING,
        EXPLODING
    };
}

namespace PlayerActions {
    enum Enum {
        ACCELERATE,
        ROTATE_RIGHT,
        ROTATE_LEFT,
        SHOOT
    };
}

const float PI = (const float) std::acos(-1);
const int GRAVITY = 1;
const int TRAIL_LENGTH = 50;
const sf::Color BACKGROUND_COLOR(180, 180, 180);
const int SCALE_FACTOR = 1;
const int GUI_SCALE_FACTOR = 4  ;
const unsigned FPS = 30;
const sf::Vector2u DISPLAY_DIMENSIONS(1920, 1080);

float to_radians(float degrees);
float to_degrees(float radians);
float find_distance(sf::Vector2f coordinates_1, sf::Vector2f coordinates_2);
sf::Vector2f find_velocity(float angle, float force);

void print_nums(sf::Vector2f vector);
void print_nums(sf::Vector2i vector);
void print_nums(sf::Vector2u vector);

template <typename t>
void print(t thing) {
    std::cout << thing << std::endl;
}

void print(std::string);
sf::Vector2f find_center(sf::Vector2f dimensions_1, sf::Vector2f dimensions_2, sf::Vector2f coordinates=sf::Vector2f());

#endif
