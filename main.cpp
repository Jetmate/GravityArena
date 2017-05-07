#include <SFML/Graphics.hpp>
#include "spritesheet.h"
#include "game.h"
#include "classes.h"

void print_nums(sf::Vector2f vector) {
    printf("%f, %f \n", vector.x, vector.y);
}

void print_nums(sf::Vector2i vector) {
    printf("%i, %i \n", vector.x, vector.y);
}

void print_nums(sf::Vector2u vector) {
    printf("%u, %u \n", vector.x, vector.y);
}

float to_radians(float degrees) {
    return degrees * (PI / 180);
}

float to_degrees(float radians) {
    return radians * (180 / PI);
}

float find_distance(sf::Vector2f coordinates_1, sf::Vector2f coordinates_2) {
    return (float) std::sqrt(
            std::pow(coordinates_1.x - coordinates_2.x, 2) + std::pow(coordinates_1.y - coordinates_2.y, 2));
}

sf::Vector2f find_velocity(float angle, float force) {
    sf::Vector2f velocity;
    velocity.x = std::cos(to_radians(angle)) * force;
    velocity.y = std::sin(to_radians(angle)) * force;
    return velocity;
}

int main() {
    const RectHitBox DISPLAY_HITBOX(DISPLAY_DIMENSIONS);
    sf::RenderWindow window(sf::VideoMode(DISPLAY_DIMENSIONS.x, DISPLAY_DIMENSIONS.y),
                            "Gravity Arena", sf::Style::Fullscreen);
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);

    SpriteSheet ship_sheet("ship_sheet.png", SCALE_FACTOR);
    SpriteSheet planet_sheet("planet_sheet.png", SCALE_FACTOR);
    SpriteSheet misc_sheet("misc_sheet.png", SCALE_FACTOR);

    std::vector<Player> players;
    std::vector<Planet> planets;

    {
        sf::Vector2u player_dimensions(25, 13);
        std::vector<std::map<int, SpriteVector>> player_sprites;
        std::map<int, SpriteVector> sprites;
        for (int i = 0; i < 2; i++) {
            sprites[PlayerSpriteTypes::IDLE] = ship_sheet.get_sprites(player_dimensions);
            sprites[PlayerSpriteTypes::ACCELERATING] = ship_sheet.get_custom_sprites(player_dimensions, 4);
            sprites[PlayerSpriteTypes::EXPLODING] = ship_sheet.get_custom_sprites(sf::Vector2u(31, 19), 4);
            player_sprites.push_back(sprites);
        }
        sf::Vector2u trail_dimensions(4, 4);
        sf::Sprite trail_sprite = misc_sheet.get_sprites(trail_dimensions)[0];
        sf::Vector2u bullet_dimensions(8, 3);
        sf::Sprite bullet_sprite = misc_sheet.get_sprites(bullet_dimensions)[0];
        sf::Vector2u health_bar_dimensions(128, 19);
        sf::Sprite health_bar_sprite = misc_sheet.get_sprites(health_bar_dimensions, 0, GUI_SCALE_FACTOR)[0];

        std::vector<std::map<int, int>> player_controls;
        std::map<int, int> controls;
        controls[sf::Keyboard::W] = PlayerActions::ACCELERATE;
        controls[sf::Keyboard::D] = PlayerActions::ROTATE_RIGHT;
        controls[sf::Keyboard::A] = PlayerActions::ROTATE_LEFT;
        controls[sf::Keyboard::Q] = PlayerActions::SHOOT;
        player_controls.push_back(controls);
        controls.clear();
        controls[sf::Keyboard::Up] = PlayerActions::ACCELERATE;
        controls[sf::Keyboard::Right] = PlayerActions::ROTATE_RIGHT;
        controls[sf::Keyboard::Left] = PlayerActions::ROTATE_LEFT;
        controls[sf::Keyboard::Slash] = PlayerActions::SHOOT;
        player_controls.push_back(controls);
        controls.clear();

        std::vector<int> player_rotations = {0, 180};
        int player_mass = 10;
        float player_movement_speed = 1.5f / FPS;
        float player_rotation_speed = 300.f / FPS;
        float player_bullet_speed = 500.f / FPS;
        int player_health = 100;
        int player_bullet_damage = 10;
        std::vector<sf::Color> health_bar_color = {sf::Color(162, 69, 69), sf::Color(58, 137, 85)};
        sf::Vector2u health_bar_margins(50, 50);
        sf::Vector2f health_bar_offset(9 * GUI_SCALE_FACTOR, 4 * GUI_SCALE_FACTOR);
        std::vector<int> player_sides = {-1, 1};

        sf::Vector2u planet_dimensions(84, 84);
        sf::Sprite planet_sprite = planet_sheet.get_sprites(planet_dimensions)[0];

        int planet_mass = 3000;


        std::vector<std::vector<sf::Vector2f>> all_player_coordinates = {
                {
                    sf::Vector2f(100, 540),
                    sf::Vector2f(1820, 540)
                },
                {
                    sf::Vector2f(1220, 500),
                    sf::Vector2f(700, 500)
                }
        };

        std::vector<std::vector<sf::Vector2f>> all_player_velocities = {
                {
                    sf::Vector2f(0, 10),
                    sf::Vector2f(0, -10)
                },
                {
                    sf::Vector2f(0, 10),
                    sf::Vector2f(0, -10)
                }
        };

        std::vector<std::vector<sf::Vector2f>> all_planet_coordinates = {
                {
                    sf::Vector2f(376, 540),
                    sf::Vector2f(960, 540),
                    sf::Vector2f(1544, 540)
                },
                {
                    sf::Vector2f(960, 540)
                }
        };

        int level = 1;
        std::vector<sf::Vector2f> player_coordinates = all_player_coordinates[level];
        std::vector<sf::Vector2f> player_velocities = all_player_velocities[level];
        std::vector<sf::Vector2f> planet_coordinates = all_planet_coordinates[level];

        for (int i = 0; i < all_player_coordinates.size(); i++) {
            players.push_back(
                    Player(
                            player_coordinates[i],
                            player_rotations[i],
                            player_dimensions,
                            player_sprites[i],
                            player_mass,
                            player_velocities[i],
                            player_movement_speed,
                            player_rotation_speed,
                            player_bullet_speed,
                            trail_sprite,
                            bullet_sprite,
                            bullet_dimensions,
                            player_controls[i],
                            player_health,
                            player_bullet_damage,
                            health_bar_dimensions,
                            health_bar_color[i],
                            player_sides[i],
                            health_bar_margins,
                            health_bar_offset,
                            health_bar_sprite
                    )
            );
        }

        for (sf::Vector2f coordinates : planet_coordinates) {
            planets.push_back(
                    Planet(
                            coordinates,
                            planet_dimensions.x / 2,
                            planet_sprite,
                            planet_mass
                    )
            );
        }

        for (Planet& planet : planets) {
            planet.update_transform();
        }
    }


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                case sf::Event::KeyReleased:
                    for (Player& player : players) {
                        if (player.is_alive() && player.in_controls(event.key.code)) {
                            bool pressed = event.type == sf::Event::KeyPressed;
                            switch (player.action_type(event.key.code)) {
                                case PlayerActions::ACCELERATE:
                                    player.accelerate(pressed);
                                    break;
                                case PlayerActions::ROTATE_RIGHT:
                                    player.turn(pressed, 1);
                                    break;
                                case PlayerActions::ROTATE_LEFT:
                                    player.turn(pressed, -1);
                                    break;
                                case PlayerActions::SHOOT:
                                    player.shoot(pressed);
                                    break;
                            }
                        }
                    }
                    break;
            }
        }

        for (auto it_player = players.begin(); it_player != players.end(); ++it_player) {
            if (it_player->is_active()) {
                if (it_player->is_alive()) {
                    it_player->accelerate();
                    it_player->turn();
                    it_player->shoot();
                    it_player->update_gravity(std::vector<MassThing>(planets.begin(), planets.end()));
                    it_player->update_coordinates();

                    for (Planet& planet : planets) {
                        if (it_player->collided(planet)) {
                            it_player->planet_collision();
                        }
                    }

                    it_player->update_bullets(DISPLAY_HITBOX);
                    it_player->bullet_collision(players, planets);
                    it_player->update_trail(planets);
                } else {
                    if (it_player->is_moving()) {
                        it_player->update_gravity(std::vector<MassThing>(planets.begin(), planets.end()));
                        it_player->update_coordinates();
                    }
                    if (it_player->update_sprites(false)) {
                        it_player->end();
                    }
                }
            }
        }

        window.clear(BACKGROUND_COLOR);

        for (Planet& planet : planets) {
            planet.display(window);
        }

        for (Player& player : players) {
            if (player.is_active()) {
                if (player.is_alive()) {
                    player.display_trail(window);
                    player.display_bullets(window);
                    player.display_health(window);
                }
                player.update_transform();
                player.display(window);
            }
            player.display_health_box(window);
        }

        window.display();
    }
    return 0;
}
