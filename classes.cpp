#include "classes.h"
#include "game.h"

Thing::Thing(sf::Vector2f coordinates, float rotation) :
        coordinates(coordinates),
        rotation(rotation)
{}

RectHitBox::RectHitBox(sf::Vector2u dimensions) :
        dimensions(dimensions),
        default_dimensions(dimensions)
{}

bool RectHitBox::collided(const Hitbox& thing) const {
    return thing.contains(coordinates)
           || thing.contains(sf::Vector2f(coordinates.x + dimensions.x, coordinates.y))
           || thing.contains(sf::Vector2f(coordinates.x, coordinates.y + dimensions.y))
           || thing.contains(coordinates + dimensions);
}

bool RectHitBox::contains(sf::Vector2f point) const {
    return point.x >= coordinates.x && point.x < coordinates.x + dimensions.x
           && point.y >= coordinates.y && point.y < coordinates.y + dimensions.y;
}

void RectHitBox::calculate_dimensions() {
    dimensions.x = (float) (cos(to_radians(rotation)) * default_dimensions.x
                            + cos(to_radians(90 - rotation)) * default_dimensions.y);
    dimensions.y = (float) (sin(to_radians(rotation)) * default_dimensions.x
                            + sin(to_radians(90 - rotation)) * default_dimensions.y);
}

CircleHitBox::CircleHitBox(int radius) :
        radius(radius)
{}

bool CircleHitBox::collided(const CircleHitBox& thing) const {
    return thing.distance_to_center(coordinates) <= radius * 2;
}

bool CircleHitBox::collided(const RectHitBox& thing) const {
    return thing.collided(*this);
}

bool CircleHitBox::contains(sf::Vector2f point) const {
    return distance_to_center(point) <= radius;
}

float CircleHitBox::distance_to_center(sf::Vector2f point) const {
    return find_distance(coordinates, point);
}

void SpriteManager::update_transform() {
    current_sprite().setPosition(coordinates);
    current_sprite().setRotation(rotation);
}

void SpriteManager::display(sf::RenderWindow& window) {
    window.draw(current_sprite());
}

SingleSpriteManager::SingleSpriteManager(sf::Sprite sprite) :
        sprite(sprite)
{}

sf::Sprite& SingleSpriteManager::current_sprite() {
    return sprite;
}

sf::Sprite& MultiSpriteManager::current_sprite() {
    return current_sprites()[sprite_index];
}

bool MultiSpriteManager::update_sprites(bool reset, int speed) {
    sprite_count += 1;
    if (sprite_count == speed) {
        sprite_count = 0;
        if (sprite_index == current_sprites().size() - 1) {
            if (reset) {
                sprite_index = 0;
            }
            return true;
        }
        sprite_index += 1;
    }
    return false;
}

SimpleMultiSpriteManager::SimpleMultiSpriteManager(SpriteVector sprites) :
        sprites(sprites)
{}

SpriteVector& SimpleMultiSpriteManager::current_sprites() {
    return sprites;
}

ComplexMultiSpriteManager::ComplexMultiSpriteManager(std::map<int, SpriteVector> sprites) :
        sprites(sprites)
{}

SpriteVector& ComplexMultiSpriteManager::current_sprites() {
    return sprites[sprite_type];
}

//SimpleThing::SimpleThing(sf::Sprite& p_sprite) {
//    sprite = p_sprite;
//    update_transform();
//    update_box();
//    dimensions = sprite.getTexture()->getSize();
//}
//
//void SimpleThing::update_transform() {
//    sprite.setPosition(coordinates);
//    sprite.setRotation(rotation);
//}
//
//void SimpleThing::update_box() {
//    bounding_box = sprite.getGlobalBounds();
//}
//
//void SimpleThing::display(sf::RenderWindow& window) const {
//    window.draw(sprite);
//}
//
//ComplexThing::ComplexThing(std::map<int, SpriteVector>& p_sprites, int p_default_sprite_type) {
//    sprites = p_sprites;
//    default_sprite_type = p_default_sprite_type;
//    sprite_type = p_default_sprite_type;
//    update_transform();
//    update_box();
//    dimensions = current_sprite().getTexture()->getSize();
//}
//
//void ComplexThing::update_transform() {
//    current_sprite().setPosition(coordinates);
//    current_sprite().setRotation(rotation);
//}
//
//void ComplexThing::update_box() {
//    update_transform();
//    bounding_box = current_sprite().getGlobalBounds();
//}
//
//void ComplexThing::display(sf::RenderWindow& window) {
//    window.draw(current_sprite());
//}
//
//SpriteVector& ComplexThing::current_sprites() {
//    return sprites.at(sprite_type);
//}
//
//bool ComplexThing::update_sprites(bool reset, int speed) {
//    sprite_count += 1;
//    if (sprite_count == speed) {
//        sprite_count = 0;
//        if (sprite_index == current_sprites().size() - 1) {
//            if (reset) {
//                sprite_index = 0;
//            }
//            return true;
//        }
//        sprite_index += 1;
//    }
//    return false;
//}
//
//sf::Sprite& ComplexThing::current_sprite() {
//    return current_sprites().at((unsigned long) sprite_index);
//}


MassThing::MassThing(int mass) :
        mass(mass)
{}

float MassThing::find_force(sf::Vector2f target_coordinates, int target_mass) const {
    return (float) (mass * target_mass * GRAVITY / std::pow(find_distance(coordinates, target_coordinates), 2));
}

float MassThing::find_angle(const sf::Vector2f target_coordinates) const {
    float degrees = to_degrees(
            std::atan(std::abs((coordinates.y - target_coordinates.y) / (coordinates.x - target_coordinates.x))));
    if (coordinates.x <= target_coordinates.x) {
        if (coordinates.y <= target_coordinates.y) {
            degrees += 180;
        } else {
            degrees = (90 - degrees) + 90;
        }
    } else if (coordinates.y <= target_coordinates.y) {
        degrees = (90 - degrees) + 270;
    }
    return degrees;
}

MovingThing::MovingThing(sf::Vector2f velocity) :
        velocity(velocity)
{}

void MovingThing::update_coordinates() {
    coordinates += velocity;
}

RotatingThing::RotatingThing(float rotation_velocity) :
        rotation_velocity(rotation_velocity)
{}

void RotatingThing::update_rotation() {
    rotation += rotation_velocity;
    if (rotation < 0) {
        rotation += 360 + 360 * (int) (rotation / -360);
    } else if (rotation > 359) {
        rotation -= 360 * (int) (rotation / 360);
    }
}

Bullet::Bullet(
        sf::Vector2f coordinates,
        float rotation,
        sf::Vector2u dimensions,
        sf::Sprite sprite,
        sf::Vector2f velocity
) :
        Thing(coordinates, rotation),
        RectHitBox(dimensions),
        SingleSpriteManager(sprite),
        MovingThing(velocity)
{}

Planet::Planet(
        sf::Vector2f coordinates,
        int radius,
        sf::Sprite sprite,
        int mass
) :
        Thing(coordinates),
        CircleHitBox(radius),
        SingleSpriteManager(sprite),
        MassThing(mass)
{}

Player::Player(
        sf::Vector2f coordinates,
        float rotation,
        sf::Vector2u dimensions,
        std::map<int, SpriteVector> sprites,
        int mass,
        sf::Vector2f velocity,
        float movement_speed,
        float rotation_speed,
        float bullet_speed,
        sf::Sprite trail_sprite,
        sf::Sprite bullet_sprite,
        sf::Vector2u bullet_dimensions,
        std::map<int, int> controls,
        int health,
        int bullet_damage,
        sf::Vector2u health_bar_sprite_dimensions,
        sf::Color health_bar_color,
        int side,
        sf::Vector2u health_bar_margins,
        sf::Vector2f health_bar_offset,
        sf::Sprite health_bar_sprite_p
) :
        Thing(coordinates, rotation),
        RectHitBox(dimensions),
        ComplexMultiSpriteManager(sprites),
        MassThing(mass),
        MovingThing(velocity),

        movement_speed(movement_speed),
        rotation_speed(rotation_speed),
        bullet_speed(bullet_speed),
        trail_sprite(trail_sprite),
        bullet_sprite(bullet_sprite),
        bullet_dimensions(bullet_dimensions),
        controls(controls),
        original_health(health),
        health(health),
        bullet_damage(bullet_damage),
        health_bar_dimensions(health_bar_sprite_dimensions
                              * (unsigned) GUI_SCALE_FACTOR
                              - sf::Vector2u(health_bar_offset * 2.f)),
        health_bar_color(health_bar_color),
        side(side),
        health_bar_sprite(health_bar_sprite_p)
{
    sf::Vector2f sprite_coordinates;
    if (side == -1) {
        sprite_coordinates = sf::Vector2f(health_bar_margins.x, DISPLAY_DIMENSIONS.y - health_bar_margins.y);
        health_bar_sprite.setOrigin(sf::Vector2f(0, health_bar_sprite_dimensions.y));
    }
    else {
        sprite_coordinates = sf::Vector2f(DISPLAY_DIMENSIONS.x - health_bar_margins.x, DISPLAY_DIMENSIONS.y - health_bar_margins.y);
        health_bar_sprite.setOrigin(sf::Vector2f(health_bar_sprite_dimensions));
    }
    health_bar_sprite.setPosition(sprite_coordinates);
    health_bar_coordinates = sprite_coordinates;
    health_bar_coordinates.y -= health_bar_offset.y;
    if (side == -1) {
        health_bar_coordinates.x += health_bar_offset.x;
    }
    else {
        health_bar_coordinates.x -= health_bar_offset.x;
    }
}

void Player::process_gravity(const MassThing& thing) {
    velocity += find_velocity((int) thing.find_angle(coordinates), thing.find_force(coordinates, mass));
}

void Player::update_gravity(const std::vector<MassThing>& things) {
    for (const MassThing& mass_thing : things) {
        process_gravity(mass_thing);
    }
}

void Player::update_trail(const std::vector<Planet>& planets) {
    trail.clear();
    sf::Vector2f old_velocity = velocity;
    sf::Vector2f old_coordinates = coordinates;
    for (int i = 0; i != TRAIL_LENGTH; i++) {
        update_gravity(std::vector<MassThing>(planets.begin(), planets.end()));
        update_coordinates();
        for (const Planet& thing : planets) {
            if (collided(thing)) {
                goto end;
            }
        }
        trail.push_back(coordinates);
    }
    end:
    coordinates = old_coordinates;
    velocity = old_velocity;
}

void Player::display_trail(sf::RenderWindow& window) {
    for (sf::Vector2f coordinates : trail) {
        trail_sprite.setPosition(coordinates);
        window.draw(trail_sprite);
    }
}

void Player::update_bullets(RectHitBox display_hitbox) {
    for (auto it_bullet = bullets.begin(); it_bullet != bullets.end();) {
        it_bullet->update_coordinates();
        if (!it_bullet->collided(display_hitbox)) {
            it_bullet = bullets.erase(it_bullet);
        } else {
            ++it_bullet;
        }
    }
}

void Player::display_bullets(sf::RenderWindow& window) {
    for (Bullet& bullet : bullets) {
        bullet.update_transform();
        bullet.display(window);
    }
}


void Player::planet_collision() {
    die();
    moving = false;
}

void Player::accelerate(bool action) {
    accelerating = action;
    if (action) {
        sprite_type = PlayerSpriteTypes::ACCELERATING;
    } else {
        sprite_type = 0;
    }
}

void Player::turn(bool action, int direction) {
    if (action) {
        turning = true;
        turning_direction = direction;
        rotation_velocity = direction * rotation_speed;
    } else if (direction == turning_direction) {
        turning = false;
    }
}

void Player::shoot(bool action) {
    shooting = action;
}

void Player::accelerate() {
    if (accelerating) {
        velocity += find_velocity(rotation, movement_speed);
    }
}


void Player::turn() {
    if (turning) {
        update_rotation();
        calculate_dimensions();
    }
}


void Player::shoot() {
    if (shooting) {
        bullets.push_back(
                Bullet(
                        coordinates,
                        rotation,
                        bullet_dimensions,
                        bullet_sprite,
                        find_velocity(rotation, bullet_speed)
                )
        );
    }
}

bool Player::in_controls(int key) const {
    return controls.find(key) != controls.end();
}

int Player::action_type(int key) const {
    return controls.at(key);
}

bool Player::is_alive() const {
    return health > 0;
}

void Player::hurt(int damage) {
    health -= damage;
    if (!is_alive()) {
        sprite_type = PlayerSpriteTypes::EXPLODING;
    }
}

void Player::die() {
    health = 0;
    sprite_type = PlayerSpriteTypes::EXPLODING;
}

bool Player::is_moving() {
    return moving;
}

bool Player::operator==(const Player& player) const {
    return player.coordinates == coordinates;
}

bool Player::operator!=(const Player& player) const {
    return player.coordinates != coordinates;
}

void Player::bullet_collision(std::vector<Player>& players, std::vector<Planet>& planets) {
    for (auto it_bullet = bullets.begin(); it_bullet != bullets.end();) {
        for (Player& player2 : players) {
            if (*this != player2) {
                if (it_bullet->collided(player2)) {
                    player2.hurt(bullet_damage);
                    it_bullet = bullets.erase(it_bullet);
                    goto end;
                }
            }
        }
        for (Planet& planet : planets) {
            if (it_bullet->collided(planet)) {
                it_bullet = bullets.erase(it_bullet);
                goto end;
            }
        }
        ++it_bullet;
        end:;
    }
}

void Player::display_health(sf::RenderWindow& window) const {
    sf::Vector2f dimensions(sf::Vector2f(
            health_bar_dimensions.x * ((float) health / original_health),
            health_bar_dimensions.y
    ));
    sf::RectangleShape health_bar(dimensions);
    sf::Vector2f border_dimensions(GUI_SCALE_FACTOR, dimensions.y);
    sf::RectangleShape border(border_dimensions);
    health_bar.setFillColor(health_bar_color);
    border.setFillColor(sf::Color::Black);
    health_bar.setPosition(health_bar_coordinates);
    border.setPosition(health_bar_coordinates.x + dimensions.x * side * -1, health_bar_coordinates.y);
    if (side == -1) {
        dimensions.x = 0;
        border_dimensions.x = 0;
    }
    health_bar.setOrigin(dimensions);
    border.setOrigin(border_dimensions);
    window.draw(health_bar);
    window.draw(border);

}

void Player::display_health_box(sf::RenderWindow& window) const {
    window.draw(health_bar_sprite);
}

void Player::end() {
    active = false;
}

bool Player::is_active() const {
    return active;
}